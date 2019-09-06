/*#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/boost/graph/graph_traits_Polyhedron_3.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/mesh_segmentation.h>
#include <CGAL/IO/output_surface_facets_to_polyhedron.h>

#include <CGAL/property_map.h>


#include <CGAL/Simple_cartesian.h>
#include <CGAL/IO/Color.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/Color.h>

#include <iostream>
#include <fstream>


template <class Refs>
struct My_face : public CGAL::HalfedgeDS_face_base<Refs> {
	CGAL::Color color;
};
// An items type using my face.
struct My_items : public CGAL::Polyhedron_items_3 {
	template <class Refs, class Traits>
	struct Face_wrapper {
		typedef My_face<Refs> Face;
	};
};

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
typedef Polyhedron::Halfedge_handle           Halfedge_handle;

int main()
{
    // create and read Polyhedron
    Polyhedron mesh;
    std::ifstream input("C:\\dev\\CGAL-4.9\\examples\\Surface_mesh_segmentation\\data\\cactus.off");
    if ( !input || !(input >> mesh) || mesh.empty() ) {
      std::cerr << "Not a valid off file." << std::endl;
      return EXIT_FAILURE;
    }

    // create a property-map for SDF values
    typedef std::map<Polyhedron::Facet_const_handle, double> Facet_double_map;
    Facet_double_map internal_sdf_map;
    boost::associative_property_map<Facet_double_map> sdf_property_map(internal_sdf_map);

    // compute SDF values using default parameters for number of rays, and cone angle
    CGAL::sdf_values(mesh, sdf_property_map);

    // create a property-map for segment-ids
    typedef std::map<Polyhedron::Facet_const_handle, std::size_t> Facet_int_map;
    Facet_int_map internal_segment_map;
    boost::associative_property_map<Facet_int_map> segment_property_map(internal_segment_map);

    // segment the mesh using default parameters for number of levels, and smoothing lambda
    // Any other scalar values can be used instead of using SDF values computed using the CGAL function
    std::size_t number_of_segments = CGAL::segmentation_from_sdf_values(mesh, sdf_property_map, segment_property_map);

    std::cout << "Number of segments: " << number_of_segments << std::endl;
    // print segment-ids
	int coutnOfFaces = 0;
	int countOfVertexes;
	for (Polyhedron::Facet_const_iterator facet_it = mesh.facets_begin();
	
        facet_it != mesh.facets_end(); ++facet_it) {
		++coutnOfFaces;
	//	facet_it->halfedge()->facet()->
        // ids are between [0, number_of_segments -1]
        std::cout << segment_property_map[facet_it] << " ";
    }
	for (Polyhedron::Vertex_const_iterator vert_it = mesh.vertices_begin(); vert_it != mesh.vertices_end(); ++vert_it) {
		
		++countOfVertexes;
	}
	//Polyhedron P;
	
    std::cout << std::endl;
	std::cout << "faces count " << coutnOfFaces << std::endl;
	std::cout << "Vertexes count " << countOfVertexes << std::endl;

    const std::size_t number_of_clusters = 4;       // use 4 clusters in soft clustering
    const double smoothing_lambda = 0.3;  // importance of surface features, suggested to be in-between [0,1]

    // Note that we can use the same SDF values (sdf_property_map) over and over again for segmentation.
    // This feature is relevant for segmenting the mesh several times with different parameters.
    CGAL::segmentation_from_sdf_values(
      mesh, sdf_property_map, segment_property_map, number_of_clusters, smoothing_lambda);


	
	std::ofstream out("C:\\dev\\CGAL-4.9\\examples\\Surface_mesh_segmentation\\data\\result.off");
	//Polyhedron output_mesh;
	//CGAL::out
	//CGAL::output_surface_facets_to_polyhedron(c2t3, output_mesh);
	out << mesh;

}
*/



#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/boost/graph/graph_traits_Polyhedron_3.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/mesh_segmentation.h>
#include <CGAL/IO/output_surface_facets_to_polyhedron.h>

#include <CGAL/property_map.h>


#include <CGAL/Simple_cartesian.h>
#include <CGAL/IO/Color.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/Color.h>


#include <CGAL/Simple_cartesian.h>
#include <CGAL/IO/Color.h>
#include <CGAL/Polyhedron_3.h>

#include <iostream>
#include <fstream>

// A face type with a color member variable.
template <class Refs>
struct My_face : public CGAL::HalfedgeDS_face_base<Refs> {
	CGAL::Color color;
};
// An items type using my face.
struct My_items : public CGAL::Polyhedron_items_3 {
	template <class Refs, class Traits>
	struct Face_wrapper {
		typedef My_face<Refs> Face;
	};
};
typedef CGAL::Simple_cartesian<double>        Kernel;
typedef CGAL::Polyhedron_3<Kernel, My_items>  Polyhedron;
typedef Polyhedron::Halfedge_handle           Halfedge_handle;














int main() {
	Polyhedron P;


	std::ifstream input("C:\\Users\\PC1\\AppData\\Roaming\\Skype\\My Skype Received Files\\3dscansAndo\\3dscansAndo\\IndraneelData\\male1.off");
	//std::ifstream input("C:\\Users\\PC1\\AppData\\Roaming\\Skype\\My Skype Received Files\\3dscansAndo\\3dscansAndo\\ColoredAndo.off");
	if (!input || !(input >>P) || P.empty()) {
		std::cerr << "Not a valid off file." << std::endl;
		return EXIT_FAILURE;
	}
	input.close();
	/*Halfedge_handle h = P.make_tetrahedron();
	h->facet()->color = CGAL::RED;
	if (h->facet()->halfedge()->facet()->color == CGAL::RED) {
		std::cout << "Face is red " << std::endl;
	}*/
	std::ofstream out("C:\\Users\\PC1\\AppData\\Roaming\\Skype\\My Skype Received Files\\3dscansAndo\\3dscansAndo\\IndraneelData\\male1ResVertColored.off", std::ofstream::out);
	out << "COFF" << "\n";
	int countOfVertexes = 0;
	int countOfFaces = 0;
	for (Polyhedron::Vertex_const_iterator vert_it = P.vertices_begin(); vert_it != P.vertices_end(); ++vert_it) {
		++countOfVertexes;
		//vert_it->halfedge()->facet()->color = CGAL::RED;
	}

	




	/*Segmentation part*/

	typedef std::map<Polyhedron::Facet_const_handle, double> Facet_double_map;
	Facet_double_map internal_sdf_map;
	boost::associative_property_map<Facet_double_map> sdf_property_map(internal_sdf_map);

	// compute SDF values using default parameters for number of rays, and cone angle
	CGAL::sdf_values(P, sdf_property_map);


	//CGAL::sdf_values(P, sdf_property_map, 1.0 / 3.0 * CGAL_PI, 5, true);

	
	// create a property-map for segment-ids
	typedef std::map<Polyhedron::Facet_const_handle, std::size_t> Facet_int_map;
	Facet_int_map internal_segment_map;
	boost::associative_property_map<Facet_int_map> segment_property_map(internal_segment_map);

	// segment the mesh using default parameters for number of levels, and smoothing lambda
	// Any other scalar values can be used instead of using SDF values computed using the CGAL function
	//std::size_t number_of_segments = CGAL::segmentation_from_sdf_values(P, sdf_property_map, segment_property_map);

	//std::cout << "Number of segments: " << number_of_segments << std::endl;
	// print segment-ids

	//Polyhedron P;

	std::cout << std::endl;
	std::cout << "faces count " << countOfFaces << std::endl;
	std::cout << "Vertexes count " << countOfVertexes << std::endl;

	const std::size_t number_of_clusters = 20;       // use 4 clusters in soft clustering
	const double smoothing_lambda =0.7;  // importance of surface features, suggested to be in-between [0,1]

	// Note that we can use the same SDF values (sdf_property_map) over and over again for segmentation.
	// This feature is relevant for segmenting the mesh several times with different parameters.

	//std::size_t number_of_segments =
	//	CGAL::segmentation_from_sdf_values(P, segment_property_map, 1.0 / 3.0 * CGAL_PI, 25,number_of_clusters, smoothing_lambda);
	//std::cout << "Number of segments with new method " <<number_of_segments<< std::endl;
	
	
	CGAL::segmentation_from_sdf_values(
		P, sdf_property_map, segment_property_map, number_of_clusters, smoothing_lambda);





	/*End of segmentation*/
	for (Polyhedron::Vertex_iterator vertex_it = P.vertices_begin(); vertex_it != P.vertices_end(); ++vertex_it) {
		vertex_it->halfedge()->face()->color = CGAL::RED;
	}
	/*for (Polyhedron::Face_iterator facet_it = P.facets_begin(); facet_it != P.facets_end(); ++facet_it) {
		++countOfFaces;
		if (segment_property_map[facet_it] == 0){
			facet_it->color = CGAL::Color(255,0,0);
		
		}
		else if (segment_property_map[facet_it] == 1){
			facet_it->color = CGAL::Color(0, 255, 0);

		} 
		else if (segment_property_map[facet_it] == 2){
			facet_it->color = CGAL::Color(0, 0, 255);

		}
		else if (segment_property_map[facet_it] == 3){
			facet_it->color = CGAL::Color(255, 255, 0);

		}
		else if (segment_property_map[facet_it] == 4){
			facet_it->color = CGAL::Color(255, 0, 255);

		}
		else if (segment_property_map[facet_it] == 5){
			facet_it->color = CGAL::Color(0, 255, 255);

		}
		else if (segment_property_map[facet_it] == 6){
			facet_it->color = CGAL::Color(255, 255, 255);

		}
		else if (segment_property_map[facet_it] == 7){
			facet_it->color = CGAL::Color(0, 0, 0);

		}
		else if (segment_property_map[facet_it] == 8){
			facet_it->color = CGAL::Color(255, 125, 125);

		}
		else if (segment_property_map[facet_it] == 6){
			facet_it->color = CGAL::Color(125, 255, 125);
		}
		else if (segment_property_map[facet_it] == 7){
			facet_it->color = CGAL::Color(125, 125, 255);
		}
		else if (segment_property_map[facet_it] == 8){
			facet_it->color = CGAL::Color(0 ,125, 125);
		}
		else if (segment_property_map[facet_it] == 9){
			facet_it->color = CGAL::Color(125, 125, 125);
		}
		else if (segment_property_map[facet_it] == 10){
			facet_it->color = CGAL::Color(50, 0, 255);
		}
		else if (segment_property_map[facet_it] == 11){
			facet_it->color = CGAL::Color(0, 50, 255);
		}
		else if (segment_property_map[facet_it] == 12){
			facet_it->color = CGAL::Color(0, 255, 50);
		}
		else if (segment_property_map[facet_it] == 13){
			facet_it->color = CGAL::Color(50, 255, 0);
		}
		else if (segment_property_map[facet_it] == 14){
			facet_it->color = CGAL::Color(255, 0, 50);
		}
		else if (segment_property_map[facet_it] == 15){
			facet_it->color = CGAL::Color(50, 0, 50);
		}
		else if (segment_property_map[facet_it] == 16){
			facet_it->color = CGAL::Color(0, 50, 50);
		}
		else if (segment_property_map[facet_it] == 17){
			facet_it->color = CGAL::Color(50, 50, 0);
		}
		else if (segment_property_map[facet_it] == 18){
			facet_it->color = CGAL::Color(100, 150, 250);
		}
		else if (segment_property_map[facet_it] == 19){
			facet_it->color = CGAL::Color(250, 150, 100);
		}
	//	else if (segment_property_map[facet_it] == 20){
	//		facet_it->color = CGAL::Color(250, 200, 100);
	//	}
	//	else if (segment_property_map[facet_it] == 21){
	//		facet_it->color = CGAL::Color(200, 250, 100);
	//	}
	//	else if (segment_property_map[facet_it] == 22){
	//		facet_it->color = CGAL::Color(170, 250, 250);
	///	}
	//	else if (segment_property_map[facet_it] == 23){
	//		facet_it->color = CGAL::Color(250, 170, 250);
	//	}
	///	else if (segment_property_map[facet_it] == 24){
	//		facet_it->color = CGAL::Color(20, 20, 100);
	//	}
	}*/

	input.open("C:\\Users\\PC1\\AppData\\Roaming\\Skype\\My Skype Received Files\\3dscansAndo\\3dscansAndo\\IndraneelData\\male1.off");
	int vertexFaceCountIndex = 0;
	std::string line;
	if (input.is_open()) {
		while (std::getline(input, line))
		{
			//std::cout << "read file " << std::endl;
			++vertexFaceCountIndex;
			if (vertexFaceCountIndex ==2){
				out << line << '\n';
				break;
			}
		}
		input.close();
	}
	//out << countOfVertexes << " " << countOfFaces << " " << 0 << "\n";
	int countOfWritedVertexes = 0;
	for (Polyhedron::Vertex_const_iterator vert_it = P.vertices_begin(); vert_it != P.vertices_end(); ++vert_it) {
		//vert_it->halfedge()->facet()->
		double vx = vert_it->point().x();
		double vy = vert_it->point().y();
		double vz = vert_it->point().z();
		//std::cout << "Current vertex " << vx << "   " << vy << "   " << vz << std::endl;
		if (countOfWritedVertexes <= countOfVertexes) {
			out << vx << " " << vy << " " << vz << " " << (int)vert_it->halfedge()->facet()->color.r() << " " << (int)vert_it->halfedge()->facet()->color.g() << " " << (int)vert_it->halfedge()->facet()->color.b() << " " << 255 << "\n";
			
		}
		++countOfWritedVertexes;
		//break;
	}
	int lineIndex = 0;
	
	input.open("C:\\Users\\PC1\\AppData\\Roaming\\Skype\\My Skype Received Files\\3dscansAndo\\3dscansAndo\\IndraneelData\\male1.off");
	//input.open("C:\\Users\\PC1\\AppData\\Roaming\\Skype\\My Skype Received Files\\3dscansAndo\\3dscansAndo\\ColoredAndo.off");
	if (input.is_open())
	{
		while (std::getline(input, line))
		{
			//std::cout << "read file " << std::endl;
			++lineIndex;
			if (lineIndex >= countOfVertexes + 3){
				out<< line << '\n';
			}
		}
		input.close();
	}
























	/*for (Polyhedron::Face_iterator facet_it = P.facets_begin();facet_it != P.facets_end(); ++facet_it) {
		//std::cout << "Face color" << (int)facet_it->halfedge()->facet()-><<"  "<<(int)facet_it->color.g()<<" "<<(int)facet_it->color.b() << std::endl;
		//std::cout << facet_it->halfedge()->vertex()->in;
		//	++coutnOfFaces;
		//	facet_it->halfedge()->facet()->
		// ids are between [0, number_of_segments -1]
		std::cout <<"Label info "<< segment_property_map[facet_it] << " ";
	}*/


	//std::ofstream out("C:\\Users\\PC1\\AppData\\Roaming\\Skype\\My Skype Received Files\\3dscansAndo\\3dscansAndo\\ColoredAndoResArtash.off", std::ofstream::out);
	//out << P;

	return 0;
}

