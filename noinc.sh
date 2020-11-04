#/bin/bash
#for dir in AbsEnv; do 
for dir in AbsEnv AbsEvent AbsEventTag AbsParm BField BaBar BbrGeom BbrStdUtils BdbTime CLHEP CMakeTools CommonUtils CrnLib DetectorModel ErrLogger Experiment FrameExtension FrameJobReport FrameLogger FrameUtil Framework GenEnv KalmanTrack MatEnv MemUtil MsgLogger PDT PacDetector PacDisplay PacEmc PacEnv PacGeom PacSim PacTest PacTrk ProbTools ProxyDict RandControl StdHepData SysUtils TrajGeom TrkBase TrkFitter TrkGeom difAlgebra stdhep; do
cd $dir
for incfile in include/*;do
  newinc=`echo ${incfile} | sed s?include/??`
#echo old $incfile new $newinc
mv $incfile $newinc
done
for srcfile in  src/*; do
  newsrc=`echo ${srcfile} | sed s?src/??`
#echo source $srcfile
mv $srcfile $newsrc
done
cd ../
done
