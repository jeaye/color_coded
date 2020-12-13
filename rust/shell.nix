{ pkgs ? import <nixpkgs> {} }:

pkgs.stdenv.mkDerivation
{
  name = "color_coded";
  buildInputs = with pkgs;
  [
    pkgs.cargo
    pkgs.rustup
    pkgs.rustfmt
  ];
}
