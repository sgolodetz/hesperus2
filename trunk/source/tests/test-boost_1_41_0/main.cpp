#include <boost/shared_ptr.hpp>
#include <boost/filesystem/operations.hpp>
using boost::shared_ptr;
using boost::filesystem::path;

int main()
{
	shared_ptr<path> blahPath(new path("./blah"));
	return 0;
}
