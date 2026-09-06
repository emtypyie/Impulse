Sandbox/
├── include/
│   ├── Vector3d.hpp       # Vector Math Engine
│   ├── body.hpp           # Data Blueprint
│   ├── Universe.hpp       # Physics Solver
│   ├── CameraControl.hpp  # Panning, zooming, and target locking
│   ├── TrailManager.hpp   # Draws fading orbital paths behind bodies
│   └── Presets.hpp        # Pre-configured solar systems/galaxy setups
├── src/
│   ├── Vector3d.cpp
│   ├── Universe.cpp
│   ├── CameraControl.cpp
│   ├── TrailManager.cpp
│   └── main.cpp           # The master interface loop & UI controls
└── resources/
    └── shaders/
        └── glow.fs        # GPU Post-processing lighting
