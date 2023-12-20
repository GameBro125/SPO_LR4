package org.example.syntax_analyzer

import org.example.lexical_analyzer.Lexeme
import org.example.lexical_analyzer.LexemeType
import org.example.syntax_analyzer.SyntacticalAnalyzer.Companion.reportError


data class Node(
    val lexeme: Lexeme? = null,
    val children: MutableList<Node> = mutableListOf()
) {
    fun addNode(node: Node) {
        children.add(node)
    }

    enum class Type {
        ASSIGN,
        PARENTHESIS,
        OPERATOR,
        OPERATOR_WITH_PARENTHESIS,
        CONSTANT_OR_NUMBER,
        CONDITIONAL,
        END,
    }
}

fun List<Lexeme>.isExpressionClosed(): Boolean {
    // ( () + () )
    // 1 21   21 0

    // ( () ) + ()
    // 1 21 0   10

    if (!first().isParenthesis() || !last().isParenthesis()) return false

    var flag = 0
    this.forEachIndexed { index, lexeme ->
        if (lexeme.value == "(") flag++
        if (lexeme.value == ")") flag--

        if (flag == 0 && index != lastIndex)
            return false
    }

    return true
}

fun List<Lexeme>.lastIndexOfFirstNode(): Int {

    var flag = 0
    this.forEachIndexed { index, lexeme ->
        if (lexeme.value == "(") flag++
        if (lexeme.value == ")") flag--

        if (flag == 0 && index != lastIndex)
            return index
    }

    reportError(value = "Что-то не так со скобками")
}

fun List<Lexeme>.assumeNodeType(): Node.Type? =
    if (isEmpty())
        null
    else if (size == 1 && first().isDelimiter())
        Node.Type.END
    else if (size > 1 && get(0).isIdentifier() && get(1).isAssign())
        Node.Type.ASSIGN
    else if (size == 1 && (first().type == LexemeType.CONSTANT || first().type == LexemeType.IDENTIFIER))
        Node.Type.CONSTANT_OR_NUMBER
    else if (!first().isParenthesis() && !last().isParenthesis() && any { it.isOperation()})
        Node.Type.OPERATOR
    else if ((first().value == "(" && last().value == ")") || get(lastIndex - 1).value == ")") {
        if (isExpressionClosed())
            Node.Type.PARENTHESIS
        else
            Node.Type.OPERATOR_WITH_PARENTHESIS
    }
//    else if (first().type == LexemeType.CONDITIONAL_OPERATOR)
//        Node.Type.CONDITIONAL
    else
        null