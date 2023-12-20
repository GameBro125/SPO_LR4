package org.example.syntax_analyzer

import org.example.lexical_analyzer.AnalyzerResult
import org.example.lexical_analyzer.Error
import org.example.lexical_analyzer.Lexeme
import org.example.lexical_analyzer.LexemeType
import org.example.syntax_analyzer.Node.Type.*

class SyntacticalAnalyzer {

    fun analyze(source: List<AnalyzerResult>): Node {
        val errors = source.filterIsInstance<Error>()
        if (errors.isNotEmpty()) {
            reportError(
                prefix = "На этапе лексического анализа были найдены ошибки:\n",
                value = errors.joinToString("\n") { it.toString() }
            )
        }
        return analyzeSyntax(source.filterIsInstance<Lexeme>(), Node())
    }

    private fun analyzeSyntax(sourceLexemes: List<Lexeme>, startNode: Node): Node {
        var lexemeList = sourceLexemes
        while (lexemeList.isNotEmpty()) {

            var delimiterIndex = lexemeList.indexOfFirst { it.type == LexemeType.DELIMITER }.takeIf { it != -1 }

            if (delimiterIndex == null) delimiterIndex = lexemeList.lastIndex

            val lexemeListBeforeDelimiter = lexemeList.subList(0, delimiterIndex + 1)
            if (delimiterIndex == -1)
                reportError(value = "${lexemeList.first().position}: начатое выражение не заканчивается разделителем <;>")

            when (lexemeListBeforeDelimiter.assumeNodeType()) {
                ASSIGN -> {
                    startNode.addNode(analyzeAssign(lexemeListBeforeDelimiter))
                    val dropCount = 2
                    lexemeList = lexemeList.drop(dropCount)
                }

                OPERATOR -> {
                    val dropCount = 3
                    lexemeList = lexemeList.drop(dropCount)
                }

                PARENTHESIS -> {
                    val dropCount = 100
                    lexemeList = lexemeList.drop(dropCount)
                }

                CONSTANT_OR_NUMBER -> TODO()
                CONDITIONAL -> TODO()
                END -> {
                    val dropCount = 1
                    lexemeList = lexemeList.drop(dropCount)
                }

                else -> {
                    val dropCount = 100
                    lexemeList = lexemeList.drop(dropCount)
                }
            }
        }
        return startNode
    }

    private fun locateConditionalBlock(lexemes: List<Lexeme>): List<Lexeme> {
        val lastElse = lexemes.findLast { it.value == "else" }
        return if (lastElse?.position != null) {
            lexemes.subList(
                fromIndex = 0,
                toIndex = lexemes.indexOfFirst { lexeme -> lexeme.type == LexemeType.DELIMITER && lexeme.position !== null && lexeme.position > lastElse.position } + 1
            )
        } else {
            lexemes.subList(
                fromIndex = 0,
                toIndex = lexemes.indexOfFirst { lexeme -> lexeme.type == LexemeType.DELIMITER } + 1
            )
        }
    }

    private fun locateParenthesisBlock(lexemes: List<Lexeme>): List<Lexeme> {
        val lastElse = lexemes.findLast { it.value == ")" }
        val lastIndex =
            if (lastElse?.position != null)
                lexemes.indexOfFirst { lexeme -> lexeme.type == LexemeType.DELIMITER && lexeme.position !== null && lexeme.position > lastElse.position } + 1
            else
                lexemes.indexOfFirst { lexeme -> lexeme.type == LexemeType.DELIMITER } + 1

        return lexemes.subList(0, lastIndex)

    }

    private fun analyzeAssign(lexemes: List<Lexeme>): Node {

        val firstLexeme = lexemes.first()
        val secondLexeme = lexemes[1]
        val assignedLexemes = lexemes.subList(2, lexemes.size - 1)

        if (!firstLexeme.isIdentifier())
            reportError(value = "${firstLexeme.position}: выражение должно начинаться с идентификатора")
        if (!secondLexeme.isAssign())
            reportError(value = "${secondLexeme.position}: вместо ${secondLexeme.value} ожидалась операция присваивания")

        val firstNode = Node(children = mutableListOf(Node(lexeme = firstLexeme)))
        val secondNode = Node(lexeme = secondLexeme)
        val thirdNode =
            // a
            if (assignedLexemes.first().isIdentifier() && assignedLexemes.size == 1)
                Node(children = mutableListOf(Node(lexeme = assignedLexemes.first())))
            // a + ...
            else if (assignedLexemes.first().isIdentifier()) {
                analyzeOperation(assignedLexemes)
            }
            // ( ...
            else if (assignedLexemes.first().isParenthesis()) {
                // ( () + () )
                if (assignedLexemes.isExpressionClosed())
                    analyzeParenthesis(assignedLexemes)
                // () + ()
                else
                    analyzeOperation(assignedLexemes, assignedLexemes.lastIndexOfFirstNode())
            }
            // ???
            else
                reportError(value = "Ожидалось выражение в скобках или идентификатор или операция")

        return Node(children = mutableListOf(firstNode, secondNode, thirdNode))
    }

    private fun analyzeParenthesis(lexemes: List<Lexeme>): Node {
        val firstLexeme = lexemes[0]
        val lastLexeme = lexemes[lexemes.lastIndex]
        val middleLexemes = lexemes.subList(1, lexemes.lastIndex)

        if (!firstLexeme.isParenthesis() || !lastLexeme.isParenthesis())
            reportError(value = "${firstLexeme.position}: ожиадалась скобочка")

        val middleNode: Node =
            // a
            if (middleLexemes.first().isIdentifierOrConstant() && middleLexemes.size == 1)
                Node(children = mutableListOf(Node(lexeme = middleLexemes.first())))
            // a + ...
            else if (middleLexemes.first().isIdentifierOrConstant()) {
                analyzeOperation(middleLexemes)
            }
            // ( ...
            else if (middleLexemes.first().isParenthesis() && middleLexemes.last().isParenthesis())
                if (middleLexemes.isExpressionClosed())
                // ( )
                    analyzeParenthesis(middleLexemes)
                else
                // ( ) + ( )
                    analyzeOperation(middleLexemes, middleLexemes.lastIndexOfFirstNode())
            else
                reportError(value = "${firstLexeme.position}: todo")


        val firstNode = Node(lexeme = firstLexeme)
        val lastNode = Node(lexeme = lastLexeme)
        return Node(children = mutableListOf(firstNode, middleNode, lastNode))
    }

    // Анализ ... + ...
    private fun analyzeOperation(lexemes: List<Lexeme>, lastIndexOfFirstNode: Int? = null): Node {

        val firstOperandLexeme = lexemes[0]
        val rightOperandLexeme = lexemes.subList((lastIndexOfFirstNode?.plus(2)) ?: 2, lexemes.lastIndex + 1)
        val operatorLexeme = lexemes.subList(lastIndexOfFirstNode ?: 0, lexemes.size).find { it.isOperation() }
            ?: reportError(value = "${lexemes.first().position}: неизвестный знак операции")

        val leftNode: Node
        val operatorNode = Node(lexeme = operatorLexeme)
        val rightNode: Node
        // a + ...
        if (firstOperandLexeme.isIdentifierOrConstant() && operatorLexeme.isOperation()) {
            leftNode = Node(children = mutableListOf(Node(firstOperandLexeme)))
            rightNode =
                    // ... a
                if (rightOperandLexeme.size == 1)
                    Node(children = mutableListOf(Node(rightOperandLexeme.first())))
                else if (rightOperandLexeme.size > 1)
                // ... a ...
                    if (rightOperandLexeme.first().isIdentifierOrConstant())
                        analyzeOperation(lexemes.subList(2, lexemes.size))
                    // ... ( ...
                    else
                        analyzeParenthesis(rightOperandLexeme)
                else
                    throw Throwable("СУКА СУКА СУКА")

        }
        // ( ...
        else if (firstOperandLexeme.isParenthesis()) {
            leftNode = analyzeParenthesis(lexemes.subList(0, lastIndexOfFirstNode!! + 1))
            rightNode =
                if (rightOperandLexeme.size == 1)
                    Node(children = mutableListOf(Node(rightOperandLexeme.first())))
                else if (rightOperandLexeme.size > 1)
                // ... a ...
                    if (rightOperandLexeme.first().isIdentifierOrConstant())
                        analyzeOperation(lexemes.subList(2, lexemes.size))
                    // ... ( ...
                    else
                        analyzeParenthesis(rightOperandLexeme)
                else
                    throw Throwable("СУКА СУКА СУКА")
        } else
            throw Throwable("СУКА СУКА СУКА")

        return Node(children = mutableListOf(leftNode, operatorNode, rightNode))
    }

    /* Анализ if ... then ... else ... */
    private fun analyzeConditional(lexemes: List<Lexeme>): Node {
        val conditionalNode = Node()

        if (lexemes.first().value != "if")
            reportError(value = "${lexemes.first().position}: условная конструкция должна начинаться с if")

        // add "if"
        conditionalNode.addNode(Node(lexeme = lexemes.first()))

        if (lexemes.none { it.value == "then" })
            reportError(value = "${lexemes.first().position}: за предикатом должно следовать ключевое слово then")

        // add "if condition"
        conditionalNode.addNode(analyzeComparison(lexemes.subList(1, lexemes.indexOfFirst { it.value == "then" })))

        // Анализ блока then ... else ...
        if (lexemes.any { it.value == "else" }) {
            // Анализ блока then ... else
            val conditionalBlock = lexemes.subList(
                fromIndex = lexemes.indexOfFirst { it.value == "then" } + 1,
                toIndex = lexemes.indexOfLast { it.value == "else" }
            )
            val innerConditionalNode = Node()
            conditionalNode.addNode(analyzeSyntax(conditionalBlock, innerConditionalNode))
            conditionalNode.addNode(Node(lexeme = lexemes.find { it.value == "else" }))
            val outerElseBlock = lexemes.subList(
                fromIndex = lexemes.indexOfLast { it.value == "else" } + 1,
                toIndex = lexemes.lastIndex + 1
            )
            // Анализ блока else ...
            analyzeSyntax(outerElseBlock, conditionalNode)
        }
        // Анализ блока then ...
        else {
            val conditionalBlock = lexemes.subList(
                fromIndex = lexemes.indexOfFirst { it.value == "then" } + 1,
                toIndex = lexemes.lastIndex + 1
            )
            analyzeSyntax(conditionalBlock, conditionalNode)
        }
        return conditionalNode
    }

    private fun analyzeComparison(lexemes: List<Lexeme>): Node {
        if (lexemes.size != 3) {
            reportError(value = "${lexemes.first().position}: некорректная форма конструкции сравнения двух элементов")
        }
        if (lexemes.first().type != LexemeType.IDENTIFIER && lexemes.first().type != LexemeType.CONSTANT) {
            reportError(value = "${lexemes.first().position}: сравниваться должны идентификаторы или константы")
        }
        val secondLexeme = lexemes[1]
        if (secondLexeme.type != LexemeType.COMPARISON_SIGN) {
            reportError(value = "${secondLexeme.position}: вместо ${secondLexeme.value} ожидался знак сравнения")
        }
        val thirdLexeme = lexemes[2]
        if (thirdLexeme.type != LexemeType.IDENTIFIER && thirdLexeme.type != LexemeType.CONSTANT) {
            reportError(value = "${thirdLexeme.position}: сравниваться должны идентификаторы или константы")
        }
        val firstNode = Node(
            children = mutableListOf(Node(lexeme = lexemes.first()))
        )
        val secondNode = Node(
            lexeme = secondLexeme
        )
        val thirdNode = Node(
            children = mutableListOf(Node(lexeme = thirdLexeme))
        )
        return Node(children = mutableListOf(firstNode, secondNode, thirdNode))
    }

    companion object {
        fun reportError(
            prefix: String = "Ошибка на позиции ",
            value: String
        ): Nothing {
            throw UnsupportedOperationException(prefix + value)
        }
    }
}