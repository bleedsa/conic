package bleed.conic.ui.setupserver

import android.app.Activity
import android.os.Build
import androidx.fragment.app.viewModels
import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.TextView
import androidx.annotation.RequiresApi
import bleed.conic.Client
import bleed.conic.R
import bleed.conic.any_null

@RequiresApi(Build.VERSION_CODES.TIRAMISU)
class SetupServer : Fragment() {
    /*
     * these are all TextViews in the ui that get set onViewCreated().
     * thus you can use them after creation without issue
     */
    var addr: TextView? = null
    var user: TextView? = null
    var pass: TextView? = null
    var submit: Button? = null
    var error: TextView? = null

    companion object {
        fun newInstance() = SetupServer()
    }

    private val viewModel: SetupServerViewModel by viewModels()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        // TODO: Use the ViewModel
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        return inflater.inflate(R.layout.fragment_setup_server, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        /* grab the ui elements from the view */
        val view = getView()
        val fnd = { id: Int -> view?.findViewById<TextView>(id) }
        /* set */
        addr = fnd(R.id.serverAddress)
        user = fnd(R.id.serverUser)
        pass = fnd(R.id.serverPassword)
        submit = view?.findViewById<Button>(R.id.serverSubmit)
        error = fnd(R.id.serverError)

        submit?.setOnClickListener {
            /* make a client */
            val cli = Client(
                activity as Activity,
                addr!!.text.toString(),
                user!!.text.toString(),
                pass!!.text.toString()
            )

            cli.write()
        }
    }
}