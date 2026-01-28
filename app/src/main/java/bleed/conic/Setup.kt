package bleed.conic

import android.content.Intent
import android.os.Build
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import androidx.annotation.RequiresApi
import bleed.conic.ui.setupserver.SetupServer

@RequiresApi(Build.VERSION_CODES.TIRAMISU)
class Setup : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.setup)

        if (savedInstanceState == null) {
            if (client_file_exists(this)) {
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