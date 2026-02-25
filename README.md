# Z-Engine


## Architecture
/simulation-engine
│
├── CMakeLists.txt / build.gradle / premake5.lua / etc
├── README.md
├── docs/
├── scripts/                  # build scripts, tooling helpers
├── third_party/              # vendored deps (if not using package manager)
│
├── core/                     # Layer 1
│   ├── include/core/
│   ├── src/
│   ├── tests/
│   └── CMakeLists.txt
│
├── modules/                  # Layer 2
│   ├── graphics/
│   │   ├── include/graphics/
│   │   ├── src/
│   │   ├── tests/
│   │   └── CMakeLists.txt
│   │
│   ├── audio/
│   ├── input/
│   ├── window/
│   ├── physics/
│   └── networking/
│
├── engine/                   # Layer 3
│   ├── runtime/              # Headless / game runtime
│   ├── editor/               # Editor application
│   ├── devtools/             # Asset pipeline, compilers
│   ├── shared/               # Engine-only shared logic
│   └── CMakeLists.txt
│
├── projects/                 # Layer 4
│   ├── sandbox/
│   ├── sample_game/
│   └── user_project_template/
│
└── tools/                    # CLI tools not part of runtime
