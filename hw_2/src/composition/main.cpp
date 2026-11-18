#include <exception>
#include <iostream>
#include <memory>

#include "AssessmentFacade.hpp"
#include "ConsoleApp.hpp"
#include "DependencyContainer.hpp"
#include "FileResultExporter.hpp"
#include "FinalGradeService.hpp"
#include "Gradebook.hpp"
#include "InMemoryEvaluationRepository.hpp"
#include "Rubric.hpp"
#include "StudentRegistry.hpp"

int main()
{
    try
    {
        DependencyContainer services;
        services.bind<StudentRepository>([](DependencyContainer&) {
            return std::make_shared<StudentRegistry>();
        });
        services.bind<EvaluationRepository>([](DependencyContainer&) {
            return std::make_shared<InMemoryEvaluationRepository>();
        });
        services.bind<Rubric>([](DependencyContainer&) {
            return std::make_shared<Rubric>();
        });
        services.bind<FinalGradeService>([](DependencyContainer&) {
            return std::make_shared<FinalGradeService>();
        });
        services.bind<Gradebook>([](DependencyContainer& container) {
            return std::make_shared<Gradebook>(container.resolve<StudentRepository>(),
                container.resolve<Rubric>(), container.resolve<EvaluationRepository>());
        });
        services.bind<AssessmentFacade>([](DependencyContainer& container) {
            return std::make_shared<AssessmentFacade>(container.resolve<StudentRepository>(),
                container.resolve<Rubric>(), container.resolve<Gradebook>(),
                container.resolve<FinalGradeService>());
        });
        services.bind<CsvResultExporter>([](DependencyContainer&) {
            return std::make_shared<CsvResultExporter>();
        });
        services.bind<TsvResultExporter>([](DependencyContainer&) {
            return std::make_shared<TsvResultExporter>();
        });
        services.bind<ConsoleApp>([](DependencyContainer& container) {
            return std::make_shared<ConsoleApp>(container.resolve<AssessmentFacade>(),
                container.resolve<CsvResultExporter>(), container.resolve<TsvResultExporter>(),
                std::cin, std::cout);
        });

        services.resolve<ConsoleApp>().run();
        return 0;
    }
    catch (const std::exception& error)
    {
        std::cerr << "Ошибка запуска: " << error.what() << '\n';
        return 1;
    }
}
