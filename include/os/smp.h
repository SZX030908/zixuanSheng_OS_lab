#ifndef SMP_H
#define SMP_H

#define NR_CPUS 2
extern void smp_init();
extern void wakeup_other_hart();
extern unsigned long get_current_cpu_id();
extern void lock_kernel();
extern void unlock_kernel();

// void smp_init();
// void wakeup_other_hart();
// unsigned long get_current_cpu_id();
// void lock_kernel();
// void unlock_kernel();

#endif /* SMP_H */
