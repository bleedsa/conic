package bleed.conic

/*
 * the setup activity is split into fragments for different stages of the setup process.
 * start with SetupServer or skip to Connect if the client configuration exists.
 */

import android.content.Intent
import android.os.Build
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import androidx.annotation.RequiresApi
import bleed.conic.ui.setupserver.SetupServer

@RequiresApi(Build.VERSION_CODES.TIRAMISU)
class Setup : AppCompatActivity() {
    val tag = "Setup Activity"

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.setup)

        if (savedInstanceState == null) {
            if (client_file_exists(this)) {
                Log.i(tag, "client file exists; switching to Connect")
                val intent = Intent(this, Connect::class.java)
                startActivity(intent)
            } else {
                supportFragmentManager.beginTransaction()
                    .replace(R.id.container, SetupServer.newInstance())
                    .commitNow()
            }
        }
    }
}