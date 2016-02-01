# ofxRedisGlob

This is a quick and easy addon for using a redis server to get and set global state across multiple pieces of software.  Values you set with `set_val` will automatically propogated to the server and keys you designate during `setup()` will automatically be kept up to date.  All at the specified period.

There's an easy-to-access redis context in this object which you can easily use for other stuff too
