# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html) (as of v7.0.0).

## [Unreleased]

### Added
- Added Visual Studio Code project files, closes #89.
- Added generic fixed-point class with storage type for bits (aka base type) templated, closes #90.

### Changed
- Improved the comments and code formatting of Fp32f.hpp.

## [v7.0.1] - 2018-01-04

### Added
- Added MIT license as LICENSE.txt, closes #88.

## [v7.0.0] - 2017-12-13

### Added
- Added enclosing `mn` namespace for all source code, closes #84.
- Added this CHANGELOG.md file, closes #85.
- Added documentation to 'docs/', closes #87.
- Added code coverage support.

### Changed
- Changed the `Fp` namespace to `MFixedPoint` (to reflect actual project name), closes #86.
- Modified header guard macros to include the text `MN_MFIXEDPOINT_...` (to include organization/project name).