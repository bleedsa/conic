package bleed.conic

import android.app.Activity
import android.content.Context
import android.os.Build
import androidx.annotation.RequiresApi
import java.io.File
import java.io.FileInputStream
import java.io.FileOutputStream


class FsFile(a: Activity, val path: String = "") {
    val ctx = a.applicationContext
    val root = ctx.filesDir
    val dir = "$root/conic"
    val file = "$dir/$path"

    fun exists(): Boolean = File(file).exists()

    fun write_str(x: String) {
        val d = File(dir)
        if (!d.exists()) {
            d.mkdirs()
        }

        val f = File(file)
        f.createNewFile()
        FileOutputStream(f).use {
            it.write(x.toByteArray())
        }
    }

    @RequiresApi(Build.VERSION_CODES.TIRAMISU)
    fun read_str(): String {
        val f = File(file)
        FileInputStream(f).use {
            val B = it.readAllBytes()
            return B.toString()
        }
    }

    override fun toString(): String {
        return "$dir/$path"
    }
}