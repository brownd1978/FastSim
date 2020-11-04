#/bin/bash
#for dir in AbsEnv; do 
for dir in AbsEnv AbsEvent AbsEventTag AbsParm BField BaBar BbrGeom BbrStdUtils BdbTime CLHEP CMakeTools CommonUtils CrnLib DetectorModel ErrLogger Experiment FrameExtension FrameJobReport FrameLogger FrameUtil Framework GenEnv KalmanTrack MatEnv MemUtil MsgLogger PDT PacDetector PacDisplay PacEmc PacEnv PacGeom PacSim PacTest PacTrk ProbTools ProxyDict RandControl StdHepData SysUtils TrajGeom TrkBase TrkFitter TrkGeom difAlgebra stdhep; do
cd $dir
sed -e "s/MatEnv/$dir/" < ../head.txt > head.txt
ls -1 *.cc *.F *.f > cfiles.txt
sed -e "s/MatEnv/$dir/" < ../tail.txt > tail.txt
cat head.txt cfiles.txt tail.txt > CMakeLists.txt
#rm head.txt tail.txt
cd ../
done
