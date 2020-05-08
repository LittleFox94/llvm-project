//===--- LFOS.h - LF OS ToolChain Implementations ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_LFOS_H
#define LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_LFOS_H

#include "Gnu.h"
#include "clang/Driver/Tool.h"
#include "clang/Driver/ToolChain.h"
#include "llvm/MC/MCTargetOptions.h"

namespace clang {
namespace driver {
namespace tools {
namespace lfos {
class LLVM_LIBRARY_VISIBILITY Linker : public Tool {
public:
  Linker(const ToolChain &TC) : Tool("lfos::Linker", "ld.lld", TC) {}

  bool hasIntegratedCPP() const override { return false; }
  bool isLinkJob() const override { return true; }

  void ConstructJob(Compilation &C, const JobAction &JA,
                    const InputInfo &Output, const InputInfoList &Inputs,
                    const llvm::opt::ArgList &TCArgs,
                    const char *LinkingOutput) const override;
};
} // end namespace lfos
} // end namespace tools

namespace toolchains {

class LLVM_LIBRARY_VISIBILITY LFOS : public ToolChain {
public:
  LFOS(const Driver &D, const llvm::Triple &Triple,
          const llvm::opt::ArgList &Args);

  bool HasNativeLLVMSupport() const override { return true; }
  bool IsIntegratedAssemblerDefault() const override { return true; }
  bool IsMathErrnoDefault() const override { return false; }
  bool useRelaxRelocations() const override { return true; };
  RuntimeLibType GetDefaultRuntimeLibType() const override {
    return ToolChain::RLT_CompilerRT;
  }
  CXXStdlibType GetDefaultCXXStdlibType() const override {
    return ToolChain::CST_Libcxx;
  }
  bool IsUnwindTablesDefault(const llvm::opt::ArgList &Args) const override {
    return false;
  }
  llvm::ExceptionHandling GetExceptionModel(const llvm::opt::ArgList &Args) const override {
    return llvm::ExceptionHandling::SjLj;
  }
  bool isPICDefault() const override { return false; }
  bool isPIEDefault() const override { return true; }
  bool isPICDefaultForced() const override { return false; }
  llvm::DebuggerKind getDefaultDebuggerTuning() const override {
    return llvm::DebuggerKind::GDB;
  }

  unsigned GetDefaultStackProtectorLevel(bool KernelOrKext) const override {
    return 2; // SSPStrong
  }

  std::string ComputeEffectiveClangTriple(const llvm::opt::ArgList &Args,
                                          types::ID InputType) const override;

  RuntimeLibType
  GetRuntimeLibType(const llvm::opt::ArgList &Args) const override;
  UnwindLibType
  GetUnwindLibType(const llvm::opt::ArgList &Args) const override;
  CXXStdlibType
  GetCXXStdlibType(const llvm::opt::ArgList &Args) const override;

  void addClangTargetOptions(const llvm::opt::ArgList &DriverArgs,
                             llvm::opt::ArgStringList &CC1Args,
                             Action::OffloadKind DeviceOffloadKind) const override;
  void
  AddClangSystemIncludeArgs(const llvm::opt::ArgList &DriverArgs,
                            llvm::opt::ArgStringList &CC1Args) const override;
  void
  AddClangCXXStdlibIncludeArgs(const llvm::opt::ArgList &DriverArgs,
                               llvm::opt::ArgStringList &CC1Args) const override;
  void AddCXXStdlibLibArgs(const llvm::opt::ArgList &Args,
                           llvm::opt::ArgStringList &CmdArgs) const override;

  const char *getDefaultLinker() const override {
    return "ld.lld";
  }

protected:
  Tool *buildLinker() const override;
};

} // end namespace toolchains
} // end namespace driver
} // end namespace clang

#endif // LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_LFOS_H
