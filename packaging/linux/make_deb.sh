#!/bin/sh
# Builds the Ubuntu/Debian package.
#   packaging/linux/make_deb.sh <folder with all-men-must-die and assets/> <out.deb> [version]
set -e
src="$1"; out="$2"; version="${3:-1.0.0}"
[ -x "$src/all-men-must-die" ] && [ -d "$src/assets" ] || { echo "usage: $0 <install folder> <out.deb> [version]" >&2; exit 1; }
here="$(cd "$(dirname "$0")" && pwd)"
stage="$(mktemp -d)"
trap 'rm -rf "$stage"' EXIT
chmod 755 "$stage"

install -d "$stage/opt/all-men-must-die" "$stage/usr/bin" "$stage/usr/share/applications" \
           "$stage/usr/share/icons/hicolor/256x256/apps" "$stage/DEBIAN"
cp "$src/all-men-must-die" "$stage/opt/all-men-must-die/"
cp -r "$src/assets" "$stage/opt/all-men-must-die/"
find "$stage/opt" -type f -exec chmod 644 {} +
chmod 755 "$stage/opt/all-men-must-die/all-men-must-die"
ln -s /opt/all-men-must-die/all-men-must-die "$stage/usr/bin/all-men-must-die"
install -m 644 "$here/all-men-must-die.desktop" "$stage/usr/share/applications/"
install -m 644 "$here/icon-256.png" "$stage/usr/share/icons/hicolor/256x256/apps/all-men-must-die.png"

cat > "$stage/DEBIAN/control" <<EOF
Package: all-men-must-die
Version: $version
Section: games
Priority: optional
Architecture: amd64
Depends: libc6 (>= 2.31), libgl1, libx11-6, libxi6, libxrandr2, libxxf86vm1
Installed-Size: $(du -sk "$stage" | cut -f1)
Maintainer: tanim913 <42853370+tanim913@users.noreply.github.com>
Homepage: https://github.com/tanim913/Valar_Morghulis
Description: All Men Must Die, a dragon-egg arcade game
 Catch the falling dragon eggs, dodge the poison clouds, beat the Night King.
 Works offline.
EOF

dpkg-deb --build --root-owner-group "$stage" "$out"
