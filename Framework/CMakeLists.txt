
add_library(Framework SHARED 

AbsModuleMemento.cc
AbsParm.cc
AbsParmBool.cc
AbsParmDouble.cc
AbsParmFilename.cc
AbsParmString.cc
AppAction.cc
AppActionApplicator.cc
AppActionCommand.cc
AppActionController.cc
AppBasicCommand.cc
AppBuild.cc
AppCloneRecorder.cc
AppCollectionInput.cc
AppCollectionInputCommand.cc
AppCommand.cc
AppConfigCommand.cc
AppConfigRecorder.cc
AppEchoCommand.cc
AppEventsCommand.cc
AppExecNode.cc
AppExecutable.cc
AppExitCommand.cc
AppFileInput.cc
AppFileInputCommand.cc
AppFileName.cc
AppFileOutputCommand.cc
AppFileOutputModule.cc
AppFileStream.cc
AppFilterModule.cc
AppFrame.cc
AppFrameMemento.cc
AppFramework.cc
AppHelpCommand.cc
AppHierarchyReportAttributes.cc
AppInputCommand.cc
AppInputModule.cc
#AppMain.cc
AppMemento.cc
AppMenu.cc
AppMethodBase.cc
AppModule.cc
AppModuleCommand.cc
AppModuleMemento.cc
AppNameNode.cc
AppNoCloneModule.cc
AppOutputCommand.cc
AppOutputModule.cc
AppPath.cc
AppPathCommand.cc
AppPathMemento.cc
AppRecord.cc
AppReportAttributes.cc
AppSequence.cc
AppSequenceCommand.cc
AppSequenceMemento.cc
AppShowCommand.cc
AppSourceFileCommand.cc
AppStop.cc
AppStopType.cc
AppStream.cc
AppStreamsOutputModule.cc
)

# set top-level directory as include root
target_include_directories(Framework PRIVATE ${PROJECT_SOURCE_DIR})

target_link_libraries(Framework FrameUtil BaBar BdbTime CLHEP ProxyDict AbsEvent CommonUtils)

# set shared library version equal to project version
set_target_properties(Framework PROPERTIES VERSION ${PROJECT_VERSION})
