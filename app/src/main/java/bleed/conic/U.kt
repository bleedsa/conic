package bleed.conic

import android.widget.TextView

fun <X> any_null(x: List<X?>): Boolean {
    for (i in x) if (i == null) return true;
    return false;
}