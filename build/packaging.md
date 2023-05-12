# Packaging for Debian
- Run `git checkout debian/latest`

Apply the patches listed in `debian/patches/series`. They should apply cleanly before the repository can be compiled.

```sh
cat debian/patches/series | xargs -I{} patch -f --strip=1 -i "debian/patches/{}"
```

To build a Debian package run the following command in a checked-out `debian/latest` branch: `gbp buildpackage --git-upstream-tree=master --git-debian-branch=debian/latest --git-ignore-new  --git-force-create --git-submodules`

The relevant build artefacts can be found using `ls ../`.

# Packaging for TailsOS
- The `tails/latest` branch is always based on the `debian/latest` branch.
- Run `git checkout tails/latest`
- Run `docker build --tag=seedtool-tails  --file build-tailsOS.Dockerfile .`
    - `/app/source` will contain a copy of the Seedtool source code.
    - `app` wiil contain all the `.deb` and source code archive files for distribution.
- Run `docker container run --name seedtool-tails -it build-seedtool-tails bash`
- Run `docker container cp seedtool-tails:/app seedtool-tails-build`
- The directory `seedtool-tails-build/` will now container the output artefacts needed for Tails.


## Possible errors and how to deal with them
