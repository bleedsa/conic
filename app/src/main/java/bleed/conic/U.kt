package bleed.conic

import android.content.Context
import android.content.Intent

/* are any of the items in this list null? */
fun <X> any_null(x: List<X?>): Boolean {
    for (i in x) if (i == null) return true;
    return false;
}

/* goto an activity from a context */
fun <X> goto_activity(x: Context, y: Class<X>) {
    val intent = Intent(x, y)
    x.startActivity(intent)
}