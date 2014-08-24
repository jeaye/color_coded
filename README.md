color_coded vim plugin
---
color_coded is a vim plugin that provides realtime (fast), tagless code highlighting using libclang. color_coded can use your [YCM](https://github.com/Valloric/YouCompleteMe) configuration for compilation options, or it can be configured separately.

  * Fast compilation, using native C++11
  * Ruby bindings (using [juble](https://github.com/jeaye/juble), a "C++14 embedded scripting framework")
  * Exhaustive customization possibilities

## Customization
There are many new highlighting groups which color_coded adds. They are designed to follow [libclang's internals](http://clang.llvm.org/doxygen/group__CINDEX.html#gaaccc432245b4cd9f2d470913f9ef0013) as closely as possible, so I'll briefly describe the most commonly used groups here.

### Member variables
```viml
hi link FieldDecl Comment
hi link MemberRef Comment
hi link MemberRefExpr Comment
```

### Macros
```viml
hi link MacroDefinition Constant
hi link MacroInstantiation Constant
```

### Numeric literals
```viml
hi link IntegerLiteral Number
hi link FloatingLiteral Float
hi link ImaginaryLiteral Number
```
