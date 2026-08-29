# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.6] - 2026-08-17

### Changed
- Bumped version to force a new upload to the Conan remote; no functional changes.
- Updated `terminus_core` dependency to `1.0.5`, `terminus_log` dependency to `1.0.4`, and `terminus_outcome` dependency to `1.0.5`.

## [1.0.5] - 2026-08-15

### Added

- Added `docker/Dockerfile.rhel10.build` for building, testing, and uploading the package on RHEL 10.
- Added `.dockerignore` to reduce the Docker build context size.

## [1.0.4] - 2026-08-14

### Fixed

* Removed the `package_id(): self.info.clear()` override. It caused Conan's `CMakeDeps` generator to emit empty `INCLUDE_DIRS`/`LIBS` data for this package whenever it was consumed as a transitive (indirect) dependency, e.g. through `terminus_image`, resulting in `terminus/math/rectangle.hpp: file not found` errors in downstream builds. The package_id is now computed normally (varies by settings/options like other Terminus libraries).
* Fixed the `get_build_info()` helper in `templates/exports.hpp.in`, which was generated inside the wrong namespace (`tmns::outcome` instead of `tmns::math`).

### Added

* Added a Conan package test at `test/package/` (`conanfile.py`, `CMakeLists.txt`, `example.cpp`) that requires the packaged `terminus_math` library, builds a `Rect2i`, and prints the library's build information.

## [1.0.3] - 2026-08-05

### Changed

* Updated OpenCV include paths from `opencv4/opencv2/...` to `opencv2/...`, restoring compatibility with Homebrew OpenCV 5.0.0.

## [1.0.2] - 2026-08-03

### Changed

* Updated `terminus_core` dependency from `1.0.2` to `1.0.3`.
* Updated `terminus_outcome` dependency from `1.0.2` to `1.0.3`.
* `terminus_log` remains at `1.0.2` (current latest).
* Switched `url` from Bitbucket to `https://github.com/Terminus-Geospatial/terminus_math`.

## [1.0.1] - 2026-01-11
### Changed
- Updated dependencies.
- Fixing issues from macos renaming task previously.

## [1.0.0] - 2026-01-10

### Changed
- Adding changelog
- Removing coordinate API as it's going to terminus_astro.
