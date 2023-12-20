package org.example.syntax_analyzer

import org.example.lexical_analyzer.Lexeme
import org.example.lexical_analyzer.LexemeType


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
        CONSTANT_OR_NUMBER,
        CONDITIONAL,
        END,
    }
}

fun List<Lexeme>.assumeNodeType(): Node.Type? =
    if (isEmpty())
        null
    else if (size == 1 && first().isDelimiter())
        Node.Type.END
    else if (size > 1 && get(0).isIdentifier() && get(1).isAssign())
        Node.Type.ASSIGN
    else if ((first().value == "(" && last().value == ")") || get(lastIndex - 1).value == ")")
        Node.Type.PARENTHESIS
    else if (!first().isParenthesis() && !last().isParenthesis())
        Node.Type.OPERATOR
    else if (size == 1 && first().type == LexemeType.CONSTANT || first().type == LexemeType.IDENTIFIER)
        Node.Type.CONSTANT_OR_NUMBER
//    else if (first().type == LexemeType.CONDITIONAL_OPERATOR)
//        Node.Type.CONDITIONAL
    else
        null