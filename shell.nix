let
  nixpkgs = fetchTarball "https://github.com/NixOS/nixpkgs/tarball/1d3aeb5a193b9ff13f63f4d9cc169fb88129f860";
  pkgs = import nixpkgs {
    config = {};
    overlays = [];
  };
in
  pkgs.mkShell {
    packages = with pkgs; [
      (pkgs.python312.withPackages (python-pkgs: [
        python-pkgs.pyserial
      ]))
      arduino
      llvmPackages_19.clang-tools
    ];
  }
