#!/bin/bash

./active_dir -s $HOME/ad_50  $HOME/ad_out "convert F_IN -resize 50% F_OUT && rm F_IN" -s $HOME/ad_20  $HOME/ad_out "convert F_IN -resize 20% F_OUT && rm F_IN" &> $HOME/active_dir.log  &

exit
