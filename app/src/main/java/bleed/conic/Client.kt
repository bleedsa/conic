package bleed.conic

import android.app.Activity
import android.os.Build
import android.util.Log
import androidx.annotation.RequiresApi
import org.json.JSONObject

val client_file_path = "Client.json"

fun client_file_exists(a: Activity): Boolean = FsFile(a, client_file_path).exists()

/* a subsonic client */
@RequiresApi(Build.VERSION_CODES.TIRAMISU)
class Client(val a: Activity, var addr: String, var user: String, var pass: String) {
    val file = FsFile(a, client_file_path)

    init {
        Log.i(tag, "opening client using config file $file")
    }

    constructor(a: Activity) : this(a, "", "", "") {
        val f = FsFile(a, "Client.json")
        val s = f.read_str()
        val j = JSONObject(s)
        addr = j["addr"] as String
        user = j["user"] as String
        pass = j["pass"] as String
    }

    val net = NetApi(addr)

    fun toJSON(): JSONObject {
        return JSONObject(mapOf("addr" to addr, "user" to user, "pass" to pass))
    }

    fun write() {
        val obj = toJSON()
        file.write_str(obj.toString())
    }
}
