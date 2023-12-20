package org.example.syntax_analyzer

import org.example.lexical_analyzer.AnalyzerResult
import org.example.lexical_analyzer.Error
import org.example.lexical_analyzer.Lexeme
import org.example.lexical_analyzer.LexemeType

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

            val delimiterIndex = lexemeList.indexOfFirst { it.type == LexemeType.DELIMITER }
            if (delimiterIndex == -1)
                reportError(value = "${lexemeList.first().position}: начатое выражение не заканчивается разделителем <;>")

            val lexemeListBeforeDelimiter = lexemeList.subList(0, delimiterIndex)
            startNode.addNode(analyzeAssign(lexemeListBeforeDelimiter))
            lexemeList = lexemeList.drop(delimiterIndex + 1)

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

        if (lexemes.first().type != LexemeType.IDENTIFIER) {
            reportError(value = "${lexemes.first().position}: выражение должно начинаться с идентификатора")
        }
        val secondLexeme = lexemes[1]
        if (secondLexeme.type != LexemeType.ASSIGN_SIGN) {
            reportError(value = "${secondLexeme.position}: вместо ${secondLexeme.value} ожидалась операция присваивания")
        }
        val thirdLexeme = lexemes[2]
        if (thirdLexeme.type != LexemeType.IDENTIFIER && thirdLexeme.type != LexemeType.CONSTANT && thirdLexeme.type != LexemeType.PARENTHESIS) {
            reportError(value = "${thirdLexeme.position}: переменной может присваиваться только другая переменная или константа")
        }

        val firstNode = Node(children = mutableListOf(Node(lexeme = lexemes.first())))
        val secondNode = Node(lexeme = secondLexeme)

        // либо (E) либо E
        val thirdNode = Node(children = mutableListOf(Node(lexeme = thirdLexeme)))
        return Node(children = mutableListOf(firstNode, secondNode, thirdNode))
    }

    // Анализ ... + ...
    private fun analyzeOperation(lexemes: List<Lexeme>): Node {
        val operationNode = Node()

        val firstLexeme = lexemes[0]
        val secondLexeme = lexemes[1]
        val thirdLexeme = lexemes[2]

        if (!firstLexeme.isIdentifierOrConstant() || !thirdLexeme.isIdentifierOrConstant())
            reportError(value = "${lexemes.first().position}: операция производится между константой и/или идентификатором")

        if (!secondLexeme.isOperation())
            reportError(value = "${lexemes.first().position}: неизвестный знак операции")




        return Node()
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

    private fun reportError(
        prefix: String = "Ошибка на позиции ",
        value: String
    ) {
        throw UnsupportedOperationException(prefix + value)
    }
}