#pragma once
#include"SimulatorCore/Task.h"
#include"Containers/Set.h"

#include<vector>
#include<cmath>
/***********************************************************
MigrationScheduler is the class defining the interface that connects
the implementation of scheduling logic with the simulator.

availableTaskIterator: Iterator that traverses the tasks 
available for execution. Corresponding to a "ready queue."

TaskIterator: iterates through all the tasks, ready or not.

isPreemptive(): Returnes the Preemptive variable value.

setTasks(Set<Task*>* tasks) initialize the iterators.

Schedule(time): A virtual function that must be implemented
by sub-classes, aka. new scheduling policies, containing
the scheduling logic.

************************************************************/

#define BOARD_ID 1
#define BOARD_MEMORY (1*pow(2,30))
#define BOARD_NB_CORE 4
#define SERVER_NB_CORE 4
#define INVALID_ID -1
#define MIGRATE_SORT_CACHE_AFFINE 1
unsigned start_time = 0;
unsigned app_count = 0;
unsigned started = 0;
unsigned mighrated = 0;
unsigned terminated_local = 0;
unsigned terminated_remote = 0;
unsigned available_cores_top = 0;
class Board;
class Task;
std::vector <Task> running_app;
std::vector <Task> migrated_app;
std::vector <Board> boards_top;

struct MigrationInstruction 
{
    bool migrate;
    int simulatorTarget;
    Task* migratedTask;
};

struct MigrationEvent
{
    int remoteComplete;
    Task* taskComplete;
};

class MigrationScheduler{
  public:	
    //vector<unsigned> boards;
    unsigned number;
    unsigned total_nb_cores;
    unsigned total_available_cores;
    std::vector<Board> boards;

  public:
     MigrationScheduler();
     void _init_boards();
     unsigned get_available_core();
     Board get_suitable_board(unsigned memory);
     std::vector<Task> sorted(std::vector<Task> running_app, unsigned key);
     unsigned migrate();
     unsigned schedule_wait();
     unsigned __migrate(Task proc, Board brd);
     unsigned update_board_info(Board brd, unsigned memory);     
};


class Board{   // for each board parameter
  private:
  public:
    unsigned nb_core;
    unsigned memory_size;
    unsigned available_core;
    unsigned free_memory;
    unsigned id;
  
  public:
    Board(unsigned m_id, unsigned m_nb_core, unsigned m_memory_size):
    nb_core(m_nb_core), memory_size(m_memory_size), available_core(m_nb_core),free_memory(memory_size), id(id) {}
  
};










