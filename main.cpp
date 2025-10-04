#include "bind.h"

#include <simskt/simskt.h>
#include <simopt/simopt.h>
#include <json.hpp>

#include <efsw/FileSystem.hpp>
#include <efsw/System.hpp>
#include <efsw/efsw.hpp>
#include <iostream>
#include <signal.h>

bool STOP = false;

void sigend( int ) {
	std::cout << std::endl << "Bye bye" << std::endl;
	STOP = true;
}

/// Processes a file action
class UpdateListener : public efsw::FileWatchListener {
  public:
	UpdateListener() {}

	std::string getActionName( efsw::Action action ) {
		switch ( action ) {
			case efsw::Actions::Add:
				return "Add";
			case efsw::Actions::Modified:
				return "Modified";
			case efsw::Actions::Delete:
				return "Delete";
			case efsw::Actions::Moved:
				return "Moved";
			default:
				return "Bad Action";
		}
	}

	void handleFileAction( efsw::WatchID watchid, const std::string& dir,
						   const std::string& filename, efsw::Action action,
						   std::string oldFilename = "" ) override {
		std::cout << "Watch ID " << watchid << " DIR ("
				  << dir + ") FILE (" +
						 ( oldFilename.empty() ? "" : "from file " + oldFilename + " to " ) +
						 filename + ") has event "
				  << getActionName( action ) << std::endl;
	}
};

efsw::WatchID handleWatchID( efsw::WatchID watchid ) {
	switch ( watchid ) {
		case efsw::Errors::FileNotFound:
		case efsw::Errors::FileRepeated:
		case efsw::Errors::FileOutOfScope:
		case efsw::Errors::FileRemote:
		case efsw::Errors::WatcherFailed:
		case efsw::Errors::Unspecified: {
			std::cout << efsw::Errors::Log::getLastErrorLog().c_str() << std::endl;
			break;
		}
		default: {
			std::cout << "Added WatchID: " << watchid << std::endl;
		}
	}

	return watchid;
}

int main( int argc, char** argv ) {
  SP::cmdopt_t cmdopt(argc, argv);
	signal( SIGABRT, sigend );
	signal( SIGINT, sigend );
	signal( SIGTERM, sigend );

  netunnel::bind_conf_t bc;

	std::cout << "Press ^C to exit demo" << std::endl;

	bool useGeneric = false;
	std::string path;

	UpdateListener* ul = new UpdateListener();

	/// create the file watcher object
	efsw::FileWatcher fileWatcher( false );

  std::string CurPath( efsw::System::getProcessPath() );

  std::cout << "CurPath: " << CurPath.c_str() << std::endl;

  /// starts watching
  fileWatcher.watch();

  /// add a watch to the system
  handleWatchID( fileWatcher.addWatch( CurPath + "test3", ul, true ) );

  /// adds another watch after started watching...
  //efsw::System::sleep( 100 );

  efsw::WatchID watchID =
    handleWatchID( fileWatcher.addWatch( CurPath + "test2", ul, true ) );

  /// delete the watch
  //fileWatcher.removeWatch( watchID );

	while ( !STOP ) {
		efsw::System::sleep( 100 );
	}

	return 0;
}
