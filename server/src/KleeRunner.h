#ifndef UNITTESTBOT_KLEERUNNER_H
#define UNITTESTBOT_KLEERUNNER_H

#include "KleeGenerator.h"
#include "ProjectContext.h"
#include "SettingsContext.h"
#include "Tests.h"
#include "streams/tests/TestsWriter.h"

#include <grpcpp/grpcpp.h>

#include <vector>

class KleeRunner {
public:
    KleeRunner(utbot::ProjectContext projectContext,
               utbot::SettingsContext settingsContext,
               fs::path serverBuildDir);
    /**
     * @brief Passes arguments to `run_klee.cpp` and executes it.
     *
     * Run Klee for test generation.
     * Pass no more than `batchSize` methods to the scrypt simultaneously.
     * @param testMethods Vector of names of testing source methods and linked bitcode files where
     * they defined.
     * @return Vector of KTestObject chunks. Each chunk contains data of
     * generated unit tests for each batch.
     * @throws ExecutionProcessException if a Clang call returns non-zero code.
     */
    void runKlee(const std::vector<tests::TestMethod> &testMethods,
                 tests::TestsMap &testsMap,
                 const std::shared_ptr<KleeGenerator>& generator,
                 const std::unordered_map<std::string, types::Type> &methodNameToReturnTypeMap,
                 const std::shared_ptr<LineInfo> &lineInfo,
                 TestsWriter *testsWriter,
                 bool isBatched,
                 bool interactiveMode);

private:
    const utbot::ProjectContext projectContext;
    const utbot::SettingsContext settingsContext;
    fs::path projectTmpPath;


    void processBatchWithoutInteractive(tests::MethodKtests &ktestChunk,
                                        const tests::TestMethod &testMethod,
                                        tests::Tests &tests);

    void processBatchWithInteractive(const std::vector<tests::TestMethod> &testMethods,
                                     tests::Tests &tests,
                                     std::vector<tests::MethodKtests> &ktests);
    fs::path getKleeMethodOutFile(const tests::TestMethod &method);
};


#endif // UNITTESTBOT_KLEERUNNER_H
