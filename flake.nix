{
  description = "Computree";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs, ... }@inputs:
    let system = "x86_64-linux";
        pkgs = nixpkgs.legacyPackages.${system};
    in {
      devShells.x86_64-linux.default = pkgs.mkShell {
        nativeBuildInputs = with pkgs; [
          # Tooling
          gcc
          cmake
          ninja

          # Dependencies
          qt5Full
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
      };
    };
}
