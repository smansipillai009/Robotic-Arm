# v1 Retro — 3-DOF Robotic Arm

Honest accounting of what v1 actually did, what stalled, and what v2 fixes. Written before any v2 work starts, so v2 doesn't quietly repeat the same gaps.

## What worked

- **State machine** — clean cycle: `IDLE → PICKING → LIFTING . dead states, manual intervention needed.
- **Autonomous detection** — HC-SR04 ultrasonic with a 2-sample debounce (both reads have to agree before triggering). Reliable enough that false triggers weren't a real problem.
- **Servo motion** — interpolated sweeps rather than instant jumps. Looked and behaved like a real mechanism, not a toy.
- **Kinematics on paper** — DOF correctly calculated via Kutzbach's criterion (3 revolute joints, open chain, DOF = 3). Forward kinematics equations derived and documented correctly.

## What stalled, and why

- **No CAD.** We never modeled the arm we actually built. There's no design we can point to and say "we made this on purpose" — the physical form was whatever was easiest to assemble, not something sized to a load case.
- **No structural analysis.** Zero mechanical engineering happened past the DOF calculation. We don't know if the links were over- or under-built, because we never checked.
- **Forward kinematics never left the README.** The equations were derived correctly but never used in code — joint angles were hardcoded magic numbers, not computed from a target position.
- **No inverse kinematics.** Direct consequence of the above — we could never tell the arm "go to point (x, y)," only "run this fixed sequence."
- **No 3D-printed parts.** Whatever the arm was built from wasn't purpose-designed, and the repo's own file structure referenced `cad/`, `analysis/`, `kinematics/` folders that never existed.
- **Repo state didn't match the README.** Documentation described a project structure the repo never actually had.

The common thread: v1 was a *control and electronics* project wearing a mechanical engineering project's name. The parts that make it mechanical — CAD, load calcs, structural verification — never happened.

## What v2 changes

| v1 gap | v2 fix |
|---|---|
| No CAD | Arm modeled in SolidWorks, dimensions driven by calculated loads — not guessed |
| No structural analysis | ANSYS static structural + mesh convergence + modal analysis on every link |
| FK derived but unused | FK/IK validated in Python first, then ported to the microcontroller |
| Hardcoded angles | Replaced entirely by IK-computed targets — hardcoded angles don't count as v2 |
| No printed parts | Parts printed to calculated cross-sections, oriented for print strength |
| Repo didn't match docs | Repo restructured (`src/`, `cad/`, `analysis/`, `kinematics/`, `docs/`) and kept in sync as we go |

v2's rule: **design → calculate → analyse → build → test**, in that order, every time. Nothing gets built before the number behind it exists.
