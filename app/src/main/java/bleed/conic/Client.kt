package bleed.conic

import android.app.Activity
import android.content.Context
import android.os.Build
import android.util.Log
import androidx.annotation.RequiresApi
import org.json.JSONObject

val client_file_path = "Client.json"

fun client_file_exists(a: Activity): Boolean = FsFile(a, client_file_path).exists()

/* a subsonic client */
@RequiresApi(Build.VERSION_CODES.TIRAMISU)
class Client(ctx: Context, var addr: String, var user: String, var pass: String) {
    val tag = "Client"
    val file = FsFile(ctx, client_file_path)
    lateinit var http: Http

    init {
        Log.i(tag, "opening client using config file $file")
        http = Http(addr)
    }

    constructor(ctx: Context) : this(ctx, "", "", "") {
        val f = FsFile(ctx, "Client.json")
        val s = f.read_str()
        Log.i(tag, "f.read_str(): $s")
        val j = JSONObject(s)
        val F = {k: String -> j[k] as String}
        addr = F("addr");user = F("user");pass = F("pass");http = Http(addr)
    }

    /* various util methods */
    fun toJSON(): JSONObject = JSONObject(mapOf("addr" to addr, "user" to user, "pass" to pass))
    fun write() = file.write_str(toJSON().toString())
    override fun toString(): String = "$user@$addr"

    fun ping(): Boolean {
        val res = http.get("/rest/ping.view", "");
        return res.status == 200
    }
}
