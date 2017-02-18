#!/bin/bash
echo "Building animations files in $PWD"

montage player-idle-{left,right}-[1-1].png -tile 1x2 -geometry 32x32+0+0 -background transparent player-idle.png
montage player-attack-{left,right}-[1-1].png -tile 1x2 -geometry 32x32+0+0 -background transparent player-attack.png

montage player-{idle,attack}.png -tile 1x2 -geometry 32x64+0+0 -background transparent player-spritesheet.png
