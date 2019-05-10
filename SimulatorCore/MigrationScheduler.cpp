#include"MigrationScheduler.h"
#include<vector>
#include<stdlib.h>
#include<iostream>
#include"TaskHandler.h"

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
    //boards.push_back(brd);
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


unsigned MigrationScheduler::retainboard(unsigned id){
  for(auto &brds:boards){
    if(brds.id == id){
      total_available_cores +=1;
      brds.available_core +=1;
      break;
    }
  }
}

unsigned MigrationScheduler::update_board_info(Board brd, unsigned memory){
// lookup for the board then update info
  for(auto &brds:boards){
    if(brds.id == brd.id){
      total_available_cores -= 1;
      brds.available_core -=1;
      brds.free_memory -= memory;
	//cout << "\nupdate_info: id"<<brds.id<<"  available core"<<brds.available_core<<endl;
	break;
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

// TODO for Sefat: Remove this and replace with your migration scheduler.
// Inputs to this function: Pointer to the Queue of migration events, if there are events, handle them, else if empty do nothing.
// Outputs expected, either MigrationInstruction with {false, 0, NULL} or if we wish to migrate, MigrationInstruction with {true, processor, Task*}
MigrationInstruction MigrationScheduler::checkMigrate(Queue<MigrationEvent*>* events){
  
  //cout << "called the check Migrate function\n";
  MigrationInstruction migZeroInstruction = {false, 0, NULL};
  static int test = 0;
  static Task* firstTask = m_localTasks->getItem(0);
  MigrationInstruction migYesInstruction = {true, 1, firstTask};
  //++test;
  // This makes us send yes migrate once, then no migration afterwards
  /*
  if (test == 30) {
      return migYesInstruction;
  }
  else
    return migZeroInstruction;
  */
  // if number of task is greather than one then migrate. 

  // print number of event
  //cout << "\nnumber of queue events: " << events->numberOfItems()<<endl;
  if(events->numberOfItems() > 0){
    for(int i = 0; i< events->numberOfItems(); i++){
	MigrationEvent *migevnt = events->getItem(i);
	//cout<<"\n board complete: "<< migevnt->remoteComplete<<endl;
	retainboard(migevnt->remoteComplete);
    }

  // empty the que after handling the events
  events->emptyQueue();    
  }
  
  // print number of tasks
  //cout << "\nnumber of Task: " << m_localTasks->numberOfItems()<<endl;

 if(m_localTasks->numberOfItems() && total_available_cores > 0){ 
  double max = -1;
  static Task* task_tosend;
  AbstractIterator<Task*>* taskIterator = m_localTasks->createIterator();

  //cout << "Finding out the max timeExchange rate! \n";
  for(taskIterator->First(); !taskIterator->IsDone(); taskIterator->Next() ){
    Task * task = taskIterator->CurrentItem();
    if(task->getExecutionTimeExchangeRate() > max){
	max = task->getExecutionTimeExchangeRate();
	task_tosend = task;
     }
	
   }
   //cout << "size of boards: "<< boards.size()<<endl;
   Board board_tosend = get_suitable_board(task_tosend->get_memory_usage());
   //cout << "Board  " << board_tosend.id << "\n";
   //cout << "Finding out board!\n"; 
   MigrationInstruction migrateinstruction = {true, board_tosend.id, task_tosend};
   migratetoboard(*task_tosend, board_tosend);
   return migrateinstruction;

   }else {


   MigrationInstruction migrationNull = {false, 0, NULL};
   return migrationNull;}
   
}



unsigned MigrationScheduler::migratetoboard(Task proc, Board brd){
  update_board_info(brd, proc.get_memory_usage());

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

