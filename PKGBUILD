# Maintainer: kamisaki
pkgname=animefetch
pkgver=0.1.0
pkgrel=1
pkgdesc="View your favorite anime details instantly in the terminal"
arch=('x86_64')
license=('GPL-3')
depends=('libcurl' 'gcc' 'cmake' 'make') 
source=("animefetch.cpp" "modules/json.hpp" "CMakeLists.txt")
sha256sums=('SKIP' 'SKIP' 'SKIP')

build() {
    cd "$srcdir"
    mkdir -p build
    cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release
    make
}

package() {
    cd "$srcdir/build"
    install -Dm755 "$srcdir/build/$(basename animefetch)" "$pkgdir/usr/bin/animefetch"
}
