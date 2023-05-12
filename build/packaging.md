If you can read this file, you should be in a checked-out `debian/latest` branch.

Apply the patches listed in `debian/patches/series`. They should apply cleanly before the repository can be compiled.
```sh
cat debian/patches/series | xargs -I{} patch --strip=1 -i "debian/patches/{}"
```
To build a Debian package run the following dommand in a checked-out `debian/latest` branch: `gbp buildpackage --git-upstream-tree=master --git-debian-branch=debian/latest --git-ignore-new  --git-force-create --git-submodules`.

# Packaging for TailsOS
- Run `git checkout tails/latest`
- Use `./build-tailsOS.Dockerfile`
    - `/app/source` will contain a copy of the Seedtool source code.
    - `app` wiil contain all the `.deb` and source code archive files for distribution.


## Possible errors and how to deal with them
