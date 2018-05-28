This is just me playing around with
[libassuan](https://www.gnupg.org/related_software/libassuan/index.en.html) and
[gpg-agent](https://www.gnupg.org/index.html).

If you want to compile the code, this is a standard autotools project:

```bash
$ ./autogen.sh
$ ./configure
$ make
```

This will produce an executable at `src/assuan-toy`.
