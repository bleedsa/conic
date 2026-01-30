package bleed.conic

import android.app.Activity
import android.content.Context
import android.os.Build
import android.util.Log
import androidx.annotation.RequiresApi
import java.io.File
import java.io.FileInputStream
import java.io.FileOutputStream
import java.nio.charset.Charset


class FsFile(ctx: Context, val path: String = "") {
    val tag = "FsFile"
    val root = ctx.filesDir
    val dir = "$root/conic"
    val file = "$dir/$path"

    fun exists(): Boolean = File(file).exists()

    fun write_str(x: String) {
        Log.i(tag, "writing file $file:\n$x")
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
        Log.i(tag, "reading file $file")
        val f = File(file)
        FileInputStream(f).use {
            val B = it.readAllBytes()
            return B.toString(Charsets.UTF_8)
        }
    }

    override fun toString(): String {
        return file
    }
}