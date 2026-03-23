# value-sequencer
A C++ library for generating predefined sequences of values for templated types.
It provides two main classes for controlling how long each value is held: either by execution count or by elapsed time.
## ScanValue class
- Purpose: Creates sequences of templated types where each value is held for a configurable number of executions.
- Usage: Ideal when you want the sequence to advance after a fixed number of steps rather than based on real time.
## TimeValue class
- Purpose: Creates sequences of templated types where each value is held for a configurable amount of time.
- Timing: Time is measured using the ITimeDuration interface, which abstracts the source of time.
- Requirements: ITimeDuration must be implemented to provide concrete time measurements. A concrete implementation is provided for standard use cases.
- Usage: Ideal when you want sequence values to persist for a real-world duration, independent of execution speed.
