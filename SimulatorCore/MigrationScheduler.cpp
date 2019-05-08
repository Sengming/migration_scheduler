#include"MigrationScheduler.h"
#include<vector>
#include<stdlib.h>
#include<iostream>


MigrationScheduler::MigrationScheduler(Set<Task*>* tasks, unsigned numberOfBoards){
        m_localTasks = tasks;
        total_nb_cores = 0;
        total_available_cores = 0;
	  number = numberOfBoards;
	//cout<<number<<endl;
        _init_boards();
	
	}

unsigned MigrationScheduler::get_available_core(){
  return total_available_cores;
}

void MigrationScheduler::_init_boards(){
  for(int i=0; i< number; i++){
    Board brd(i,BOARD_NB_CORE,BOARD_MEMORY);
    total_nb_cores += brd.nb_core;
    boards.push_back(brd);
    boards_top.push_back(brd);
  }
  total_available_cores = total_nb_cores;
  available_cores_top = total_available_cores;
}

Board MigrationScheduler::get_suitable_board(unsigned memory){
  if (total_available_cores==0){
    Board brd(-1,-1,-1);
    return brd;
  }
  for(auto &brd:boards){
    if(brd.available_core > 0 && brd.free_memory >= memory){
    return brd;
    }
  }

}



unsigned MigrationScheduler::schedule_wait(){
  if(running_app.size() < SERVER_NB_CORE){
    return -1;
  }

}

unsigned MigrationScheduler::update_board_info(Board brd, unsigned memory){
// lookup for the board then update info
  for(auto &brds:boards){
    if(brds.id == brd.id){
      total_available_cores -= 1;
      brds.available_core -=1;
      brds.free_memory -= memory;
    }
  }

}

// MigrationInstruction MigrationScheduler::checkMigrate(Queue<MigrationEvent*>* events){
//   MigrationInstruction migInstruction = {false, 0, NULL};
//   if(get_available_core() <= 0){
//     return migInstruction;
//   }

//   vector<Task> running_app_sorted = sorted(running_app, MIGRATE_SORT_CACHE_AFFINE);
//   for(auto &proc:running_app_sorted){
//     Board dest =  get_suitable_board(proc.get_memory_usage());
//     __migrate(proc,dest);
//   }


// }

MigrationInstruction MigrationScheduler::checkMigrate(Queue<MigrationEvent*>* events){
  //MigrationInstruction migInstruction = {false, 0, NULL};
  static Task* firstTask = m_localTasks->getItem(0);
  MigrationInstruction migInstruction = {true, 1, firstTask};
  return migInstruction;
}

unsigned MigrationScheduler::__migrate(Task proc, Board brd){
  // update the board info
  update_board_info(brd, proc.get_memory_usage());
  // remove the app from running app 
  
  for(auto it = running_app.begin(); it != running_app.end(); ++it){
    if(it->getID() == proc.getID()){
      running_app.erase(it);
    }
  }
  // add it to migrated app
  migrated_app.push_back(proc);
}


vector<Task> MigrationScheduler::sorted(vector<Task> running_app, unsigned key){
  unsigned score = 1; // need to implemnt scoring function;
  vector<Task> sorted;
  switch(key){
    case MIGRATE_SORT_CACHE_AFFINE:

	return sorted;
      break;
  
  }

}

