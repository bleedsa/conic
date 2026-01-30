package bleed.conic

class HttpResult(val status: Int, val body: String) {}

class Http(val addr: String) {
    external fun get_(addr: String, path: String, query: String): HttpResult

    fun get(path: String, query: String): HttpResult = get_(addr, path, query)
}