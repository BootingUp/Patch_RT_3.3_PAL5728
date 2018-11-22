#! /bin/sh 
LOG=audio.log
amixer cset name="Left PGA Mixer Line1L Switch" 1 >>$LOG 2>&1
amixer cset name="Left PGA Mixer Line1R Switch" 1 >>$LOG 2>&1
amixer cset name="Left PGA Mixer Mic2L Switch" 1 >>$LOG 2>&1
amixer cset name="Left PGA Mixer Mic2R Switch" 1 >>$LOG 2>&1

amixer cset name="Right PGA Mixer Line1L Switch" 1 >>$LOG 2>&1
amixer cset name="Right PGA Mixer Line1R Switch" 1 >>$LOG 2>&1
amixer cset name="Right PGA Mixer Mic2L Switch" 1 >>$LOG 2>&1
amixer cset name="Right PGA Mixer Mic2R Switch" 1 >>$LOG 2>&1

amixer cset name="PGA Capture Volume" 30% >> $LOG 2>&1
amixer cset name="PCM Playback Volume" 80% >> $LOG 2>&1
amixer cset name="Left DAC Mux"  1 >> $LOG 2>&1
amixer cset name="Right DAC Mux" 1 >> $LOG 2>&1

