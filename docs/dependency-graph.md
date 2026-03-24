# Zen Engine Internal Dependencies

## Dependency Rules
Higher layers can depend on lower layers, but not vice versa.

## Layer Dependencies

### Core Layer (zen::core)
- May include other core files
- External use only lightweight external header-only libraries
- Used by: All other layers

### Modules Layer (zen::modules::<module_name>)
- Depends on: Core Layer
- Used by: Engine, Projects

### Engine Layer (zen::engine::<system>)
- Depends on: Core Layer
- Depends on: Modules Layer (optional)
- Used by: Projects

### Projects Layer
- Depends on: Core Layer
- Depends on: Modules Layer (as needed)
- Depends on: Engine Layer (as needed)

