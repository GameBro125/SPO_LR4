package org.example

import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.PaddingValues
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.wrapContentSize
import androidx.compose.material.MaterialTheme
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.text.TextStyle
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.unit.TextUnit
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.compose.ui.window.Window
import androidx.compose.ui.window.application
import androidx.compose.ui.window.rememberWindowState
import cafe.adriel.bonsai.core.Bonsai
import cafe.adriel.bonsai.core.BonsaiStyle
import cafe.adriel.bonsai.core.node.Branch
import cafe.adriel.bonsai.core.node.Leaf
import cafe.adriel.bonsai.core.tree.Tree
import cafe.adriel.bonsai.core.tree.TreeScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.runBlocking
import org.example.lexical_analyzer.AnalyzerResult
import org.example.lexical_analyzer.LexicalAnalyzer
import org.example.optimisation.OptimisationExecutable
import org.example.syntax_analyzer.SyntacticalAnalyzer
import org.example.syntax_analyzer.Node
import org.example.tables.BinaryTreeTable
import tables.SimpleRehashTable
import kotlin.time.measureTime


import java.io.BufferedReader
import java.io.InputStreamReader

fun main() {
        val optimisation = OptimisationExecutable()
        optimisation.rules()
}














//fun main() = application {
//    Window(
//        onCloseRequest = ::exitApplication,
//        title = "ЛР3",
//        state = rememberWindowState(width = 1000.dp, height = 1000.dp)
//    ) {
//        MaterialTheme {
//            Box(modifier = Modifier.fillMaxSize())
//            { testAnalyzer() }
//        }
//    }
//}

/*

    if a > b
        then if b > c
            then if c > d
                then c := III;
                else c := VVV;
            else c := XXX;

    x := ((a + XV) * (VV / z ));

*/
//
//@Composable
//fun testAnalyzer() {
//    val sourceCode = """
//        x := VVV;
//        x := (VVV);
//    """.trimIndent()
//    val lexicalResults = analyzeLexemes(sourceCode)
//    analyzeSyntax(lexicalResults)
//        .onSuccess { drawNode(node = it) }
//        .onFailure { println(it.printStackTrace()) }
//}
//
//@Composable
//fun TreeScope.drawBranch(node: Node) {
//    if (node.children.isEmpty()) {
//        Leaf(node.lexeme?.value ?: "E")
//    } else {
//        Branch(node.lexeme ?: "E") {
//            node.children.forEach { drawBranch(it) }
//        }
//    }
//}
//
//@Composable
//fun drawNode(node: Node) {
//    val tree = Tree<String> {
//        drawBranch(node)
//    }
//    Bonsai(
//        tree = tree,
//        modifier = Modifier.wrapContentSize(),
//        style = BonsaiStyle(
//            nodeNameStartPadding = 12.dp,
//            nodePadding = PaddingValues(all = 12.dp),
//            nodeNameTextStyle = TextStyle(
//                fontSize = 20.sp,
//                fontWeight = FontWeight(1000),
//            )
//        )
//    )
//}
