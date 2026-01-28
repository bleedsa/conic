package bleed.conic

import android.util.Log
import org.json.JSONObject
import java.io.BufferedReader
import java.io.InputStreamReader
import java.net.URL
import java.net.HttpURLConnection

const val tag = "NetApi"

class NetApi(val addr: String) {
    fun mkGETcli(path: String): HttpURLConnection {
        Log.i(tag, "making request at $addr$path")
        val url = URL(addr + path);
        val charset = "UTF-8"
        val query = ""
        val cli = url.openConnection() as HttpURLConnection
        cli.requestMethod = "GET"
        return cli
    }

    fun req(path: String, method: String = "GET"): JSONObject {
        val cli = when (method) {
            else -> mkGETcli(path)
        }
        val code = cli.responseCode
        Log.i(tag, "got response code $code from request")
        if (code != HttpURLConnection.HTTP_OK)
            throw RuntimeException("req($path, $method) returned status $code")

        val r = BufferedReader(InputStreamReader(cli.getInputStream()))
        val str = r.readText()
        r.close()
        cli.disconnect()

        return JSONObject(str)
    }
}