## 🏗️ Building the Plugin

### Method 1: qmake Build (Plugin Only)

```bash
# Navigate to FluentWidgetsPlugin directory
cd shared/FluentWidgetsPlugin

# Build plugin using qmake
qmake FluentWidgetPlugin.pro
nmake release              # Windows
# make release             # Linux/macOS

# Install plugin  
nmake install
# OR manually copy:
copy release\fluentwidgetsplugin.dll E:\Qt\6.8.3\msvc2022_64\plugins\designer\
```

### Method 2: CMake Build (Library + Plugin)

```bash
# From FluentWidgetsPlugin directory
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=E:/Qt/6.8.3/msvc2022_64
cmake --build . --config Release

# Copy plugin to Designer
copy FluentWidgetsPlugin.dll E:\Qt\6.8.3\msvc2022_64\plugins\designer\
```

## ✅ Verification

### 1. Test in Qt Designer
- **Launch Designer**: `designer.exe`
- **Check Widget Box**: Look for **"Fluent Widgets"** group
- **Should see**: CardWidget with live preview
- **Test**: Drag CardWidget to form, edit properties

### 2. Test in Your Application
Update your main project's CMakeLists.txt:
```cmake
# In connect-maintena/CMakeLists.txt
target_link_libraries(connect-maintena PRIVATE
    Qt6::Core
    Qt6::Widgets  
    FluentWidgetLib  # ← Changed from CardWidgetLib
)
```