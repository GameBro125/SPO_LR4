package org.example.optimisation

import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.runBlocking
import java.io.BufferedReader
import java.io.InputStreamReader

class OptimisationExecutable {

    fun rules() = runBlocking(Dispatchers.Default) {
        try {
            // Указываем путь к исполняемому файлу
            val command = "C:\\Users\\Wladislaw\\Desktop\\SPO_LR4\\src\\main\\cpp\\x64\\Debug\\LR4.exe"

            // Создаем объект ProcessBuilder с командой
            val processBuilder = ProcessBuilder(command)

            // Запускаем процесс
            val process = processBuilder.start()

            // Получаем входной поток, чтобы прочитать вывод
            val inputStream = process.inputStream
            val bufferedReader = BufferedReader(InputStreamReader(inputStream))

            // Читаем и выводим вывод исполняемого файла
            bufferedReader.useLines { lines ->
                lines.forEach { println(it) }
            }

            // Дожидаемся завершения процесса
            val exitCode = process.waitFor()
            println("Exit code: $exitCode")

        } catch (e: Exception) {
            e.printStackTrace()
        }
    }
}