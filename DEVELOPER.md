# Developer Readme

Information about development matters related to Obsidian.

## Releasing a New Version

To release a new version, do the following:

- Make changes to Obsidian.
- Document changes in `README.md` in the changelog.
- Update version number in `src/js/pebble-js-app.js` and `appinfo.json`.
- Update table in `README.md` that relates app version numbers with configuration version numbers.
    - Publish a new branch `config-X` if a new configuration version `X` was necessary.
- Build a clean version: `make clean build`.
- Upload and publish new version.

## Pebble Screen Sizes

- Pebble Original: 144x168
- Pebble Time: 144x168
- Pebble Round: 180x180
- Pebble 2: 144x168
- Pebble Time 2: 200x228
