package bleed.conic

import android.os.Build
import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.annotation.RequiresApi
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.ui.Modifier
import bleed.conic.ui.theme.ConicTheme

@RequiresApi(Build.VERSION_CODES.TIRAMISU)
class Connect : ComponentActivity() {
    lateinit var cli: Client
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        cli = Client(applicationContext)
        val p = cli.ping()

        enableEdgeToEdge()
        setContent {
            ConicTheme {
                Scaffold { padding ->
                    Column(
                        modifier = Modifier.fillMaxSize().padding(padding)
                    ) {
                        Text("ping Client of $cli...${if (p) "ok" else "err"}")
                    }
                }
            }
        }

        if (p) {
            goto_activity(this, Home::class.java)
        }
    }
}