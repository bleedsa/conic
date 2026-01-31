package bleed.conic

import android.app.Activity
import android.content.Context
import android.os.Build
import android.util.Log
import androidx.annotation.RequiresApi
import org.json.JSONObject
import java.net.URLEncoder
import java.nio.charset.StandardCharsets

val client_file_path = "Client.json"
val client_id = "Conic/${VERSION()}"

fun client_file_exists(a: Activity): Boolean = FsFile(a, client_file_path).exists()

/* a subsonic client */
@RequiresApi(Build.VERSION_CODES.TIRAMISU)
class Client(ctx: Context, var addr: String, var user: String, var pass: String) {
    val tag = "Client"
    val subsonic_version = "1.16.1"
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

    /* urlencode a string */
    fun enc(x: String): String =
        URLEncoder
            .encode(x, StandardCharsets.UTF_8)
            .replace("\\+", "%20")

    /* make a query string out of a map of parameters */
    fun mkquery(m: Map<String, String>): String {
        var r = ""
        var i = 0
        val keys = m.keys.toList()

        /* process a key in keys at i */
        val f = { i: Int ->
            val k = keys[i]
            val v = m[k]!!
            r += enc(k)
            r += '='
            r += enc(v)
        }

        /* process all but the last key */
        while (i < keys.size - 1) {
            f(i)
            r += '&'
            i++
        }

        /* tack the last one on at the end */
        f(keys.size-1)

        return r
    }

    fun subsonic_query(): String  {
        val (salt, hash) = password_encode(pass)
        return mkquery(mapOf(
            "u" to user,
            "t" to hash,
            "s" to salt,
            "v" to subsonic_version,
            "c" to client_id,
            "f" to "json",
        ))
    }

    fun ping(): Boolean {
        val Q = subsonic_query()
        val res = http.get("/rest/ping.view", Q);
        return res.status == 200
    }
}
