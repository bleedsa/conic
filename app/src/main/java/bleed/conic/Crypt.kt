package bleed.conic

import android.util.Log
import java.security.MessageDigest
import java.security.SecureRandom

val TAG = "Crypt"

fun mksalt(): ByteArray {
    val rng = SecureRandom.getInstance("SHA1PRNG")
    var bytes: ByteArray = byteArrayOf(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
    rng.nextBytes(bytes)
    return bytes
}

fun mkhash(salt: ByteArray, x: String): ByteArray {
    val md = MessageDigest.getInstance("MD5")
    md.update(salt)
    val bytes = md.digest(x.toByteArray())
    return bytes
}

fun password_encode(x: String): Pair<String, String> {
    val salt = mksalt()
    val hash = mkhash(salt, x)
    Log.i(TAG, "salt: $salt, hash: $hash")
    return Pair(bytes_hex(hash), bytes_hex(salt))
}