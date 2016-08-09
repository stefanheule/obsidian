#!/bin/bash

version=$(cat src/js/pebble-js-app.js |& grep "stefanheule.com/obsidian/config/" | sed "s/.*config\/\([0-9]*\).*/\1/")

ssh linode "sudo rm -rf /home/stefan/www/pages/common/data/obsidian/config/$version"
