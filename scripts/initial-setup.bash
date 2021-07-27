#!/bin/bash
################################################################################
# Make sure your pi is set up with what it needs to run GPIO and whatever else.
################################################################################

# add user to gpio group.
sudo usermod -a -G gpio $USER

# add user to the dialout group.  I think Arduino IDE needed it to use Rx/Tx, so Pi might, too
sudo adduser $USER dialout

