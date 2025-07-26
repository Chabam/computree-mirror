{
  description = "Computree";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs, ... }@inputs:
    let system = "x86_64-linux";
        pkgs = nixpkgs.legacyPackages.${system};
        qt = pkgs.qt5.qtbase;
    in {
      devShells.x86_64-linux.default = pkgs.mkShell {
        nativeBuildInputs = with pkgs; [
          # Tooling
          gcc
          cmake
          ninja

          # Dependencies
          qt
          libGLU
          eigen
          gdal
          muparser
          opencv
          pcl
          boost
          gsl
          qhull
          flann
          laszip
        ];
        QT_QPA_PLATFORM_PLUGIN_PATH = "${qt.bin}/lib/qt-${qt.version}/plugins/platforms";
      };
    };
}
