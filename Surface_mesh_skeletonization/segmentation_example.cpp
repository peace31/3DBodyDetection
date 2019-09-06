#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_items_with_id_3.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/boost/graph/graph_traits_Polyhedron_3.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/extract_mean_curvature_flow_skeleton.h>
#include <CGAL/mesh_segmentation.h>



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


#include <CGAL/IO/Color.h>

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


typedef CGAL::Simple_cartesian<double>        Kernel1;
typedef CGAL::Polyhedron_3<Kernel1, My_items>  Polyhedron1;
typedef Polyhedron1::Halfedge_handle           Halfedge_handle1;


typedef CGAL::Simple_cartesian<double>                               Kernel;
typedef Kernel::Point_3                                              Point;
typedef CGAL::Polyhedron_3<Kernel, CGAL::Polyhedron_items_with_id_3> Polyhedron;

typedef boost::graph_traits<Polyhedron>::vertex_descriptor           vertex_descriptor;
typedef boost::graph_traits<Polyhedron>::halfedge_descriptor         halfedge_descriptor;
typedef boost::graph_traits<Polyhedron>::face_descriptor             face_descriptor;

typedef CGAL::Mean_curvature_flow_skeletonization<Polyhedron>        Skeletonization;
typedef Skeletonization::Skeleton                                    Skeleton;

typedef Skeleton::vertex_descriptor                                  Skeleton_vertex;

// Property map associating a facet with an integer as id to an
// element in a vector stored internally
template<class ValueType>
struct Facet_with_id_pmap
    : public boost::put_get_helper<ValueType&,
             Facet_with_id_pmap<ValueType> >
{
    typedef face_descriptor key_type;
    typedef ValueType value_type;
    typedef value_type& reference;
    typedef boost::lvalue_property_map_tag category;

    Facet_with_id_pmap(
      std::vector<ValueType>& internal_vector
    ) : internal_vector(internal_vector) { }

    reference operator[](key_type key) const
    { return internal_vector[key->id()]; }
private:
    std::vector<ValueType>& internal_vector;
};


int main(int argc, char* argv[])
{
  std::ifstream input("C:\\Users\\PC1\\AppData\\Roaming\\Skype\\My Skype Received Files\\3dscansAndo\\3dscansAndo\\IndraneelData\\female5.off");
  Polyhedron tmesh;
  input >> tmesh;

  // extract the skeleton
  Skeleton skeleton;
  std::cout << "Read mesh " << std::endl;
  CGAL::extract_mean_curvature_flow_skeleton(tmesh, skeleton);
 
  // init the polyhedron simplex indices
  CGAL::set_halfedgeds_items_id(tmesh);
  
  //for each input vertex compute its distance to the skeleton
  std::vector<double> distances(num_vertices(tmesh));
  int skeletonPointsCount = 0;
  std::ofstream outSkeleton("C:\\Users\\PC1\\AppData\\Roaming\\Skype\\My Skype Received Files\\3dscansAndo\\3dscansAndo\\IndraneelData\\female5Skeleton.xyz", std::ofstream::out);
  
  BOOST_FOREACH(Skeleton_vertex v, vertices(skeleton) )
  {
	 
    const Point& skel_pt = skeleton[v].point;
//	std::cout << "Skeleton point " << skel_pt.x() << "   " << skel_pt.y() << "  " << skel_pt.z() << std::endl;
//	if (skeletonPointsCount < 100) {
		outSkeleton << skel_pt.x() << " " << skel_pt.y() << " " << skel_pt.z() << "\n";
	//}
    BOOST_FOREACH(vertex_descriptor mesh_v, skeleton[v].vertices)
    {
      const Point& mesh_pt = mesh_v->point();
      distances[mesh_v->id()] = std::sqrt(CGAL::squared_distance(skel_pt, mesh_pt));
    }
	++skeletonPointsCount;
  }
  outSkeleton.close();
  std::cout << "Number of skeleton points " << skeletonPointsCount << std::endl;
  // create a property-map for sdf values
  std::vector<double> sdf_values( num_faces(tmesh) );
  Facet_with_id_pmap<double> sdf_property_map(sdf_values);

  // compute sdf values with skeleton
  BOOST_FOREACH(face_descriptor f, faces(tmesh))
  {
    double dist = 0;
    BOOST_FOREACH(halfedge_descriptor hd, halfedges_around_face(halfedge(f, tmesh), tmesh))
      dist+=distances[target(hd, tmesh)->id()];
    sdf_property_map[f] = dist / 7.;
  }

  // post-process the sdf values
  CGAL::sdf_values_postprocessing(tmesh, sdf_property_map);

  // create a property-map for segment-ids (it is an adaptor for this case)
  std::vector<std::size_t> segment_ids( num_faces(tmesh) );
  Facet_with_id_pmap<std::size_t> segment_property_map(segment_ids);

  // segment the mesh using default parameters
  std::cout << "Number of segments: "
            << CGAL::segmentation_from_sdf_values(tmesh, sdf_property_map, segment_property_map) <<"\n";

  int countOfFacesInit = 0;
  
  for (Polyhedron::Face_iterator facet_it = tmesh.facets_begin(); facet_it != tmesh.facets_end(); ++facet_it)
  {
	  ++countOfFacesInit;
	  //std::cout << "Current face index " << segment_property_map[facet_it] << std::endl;;
  }
  /*Artash changed added  */
  int* faceSegmentArray = new int[countOfFacesInit];
  countOfFacesInit = 0;
  for (Polyhedron::Face_iterator facet_it = tmesh.facets_begin(); facet_it != tmesh.facets_end(); ++facet_it)
  {
	  faceSegmentArray[countOfFacesInit] = segment_property_map[facet_it];
	  ++countOfFacesInit;
	  //std::cout << "Current face index " << segment_property_map[facet_it] << std::endl;;
  }
  Polyhedron1 P;

  
  std::ifstream input1("C:\\Users\\PC1\\AppData\\Roaming\\Skype\\My Skype Received Files\\3dscansAndo\\3dscansAndo\\IndraneelData\\female5.off");
  //std::ifstream input("C:\\Users\\PC1\\AppData\\Roaming\\Skype\\My Skype Received Files\\3dscansAndo\\3dscansAndo\\ColoredAndo.off");
  if (!input1 || !(input1 >> P) || P.empty()) {
	  std::cerr << "Not a valid off file." << std::endl;
	  return EXIT_FAILURE;
  }
  input1.close();
  /*Halfedge_handle h = P.make_tetrahedron();
  h->facet()->color = CGAL::RED;
  if (h->facet()->halfedge()->facet()->color == CGAL::RED) {
  std::cout << "Face is red " << std::endl;
  }*/
  std::ofstream out("C:\\Users\\PC1\\AppData\\Roaming\\Skype\\My Skype Received Files\\3dscansAndo\\3dscansAndo\\IndraneelData\\female5ResLastVertexSkeleton.off", std::ofstream::out);
  out << "COFF" << "\n";
  int countOfVertexes = 0;
  int countOfFaces = 0;

  
 




  typedef std::map<Polyhedron1::Facet_const_handle, double> Facet_double_map;
  Facet_double_map internal_sdf_map1;
  boost::associative_property_map<Facet_double_map> sdf_property_map1(internal_sdf_map1);

  // compute SDF values using default parameters for number of rays, and cone angle
  CGAL::sdf_values(P, sdf_property_map1);


  //CGAL::sdf_values(P, sdf_property_map, 1.0 / 3.0 * CGAL_PI, 5, true);


  // create a property-map for segment-ids
  typedef std::map<Polyhedron1::Facet_const_handle, std::size_t> Facet_int_map;
  Facet_int_map internal_segment_map1;
  boost::associative_property_map<Facet_int_map> segment_property_map1(internal_segment_map1);

  // segment the mesh using default parameters for number of levels, and smoothing lambda
  // Any other scalar values can be used instead of using SDF values computed using the CGAL function
  //std::size_t number_of_segments = CGAL::segmentation_from_sdf_values(P, sdf_property_map, segment_property_map);

  //std::cout << "Number of segments: " << number_of_segments << std::endl;
  // print segment-ids

  //Polyhedron P;

  std::cout << std::endl;
  std::cout << "faces count " << countOfFaces << std::endl;
  std::cout << "Vertexes count " << countOfVertexes << std::endl;

  const std::size_t number_of_clusters = 22;       // use 4 clusters in soft clustering
  const double smoothing_lambda = 0.7;  // importance of surface features, suggested to be in-between [0,1]

  // Note that we can use the same SDF values (sdf_property_map) over and over again for segmentation.
  // This feature is relevant for segmenting the mesh several times with different parameters.

  //std::size_t number_of_segments =
  //	CGAL::segmentation_from_sdf_values(P, segment_property_map, 1.0 / 3.0 * CGAL_PI, 25,number_of_clusters, smoothing_lambda);
  //std::cout << "Number of segments with new method " <<number_of_segments<< std::endl;


  CGAL::segmentation_from_sdf_values(
	  P, sdf_property_map1, segment_property_map1, number_of_clusters, smoothing_lambda);



  std::cout << "First face color " << faceSegmentArray[0] << std::endl;
  
  for (Polyhedron1::Vertex_iterator vert_it = P.vertices_begin(); vert_it != P.vertices_end(); ++vert_it) {
	  ++countOfVertexes;
	  //vert_it->halfedge()->facet()->color = CGAL::RED;
	  if (vert_it->halfedge()->vertex()->point().x() > 100  && vert_it->halfedge()->vertex()->point().y() > 0 && vert_it->halfedge()->vertex()->point().z() > 500  ){
		  vert_it->halfedge()->face()->color = CGAL::Color(255, 0, 0);
	  }
  }

  for (Polyhedron1::Face_iterator facet_it = P.facets_begin(); facet_it != P.facets_end(); ++facet_it) {
	
	/*  if (1) {
		  continue;
	  }*/
	  if (/*segment_property_map1[facet_it]*/ faceSegmentArray[countOfFaces] == 0){
		  
		  facet_it->color = CGAL::Color(255, 0, 0);
	  }
	  else if (/*segment_property_map1[facet_it]*/faceSegmentArray[countOfFaces] == 1){
		  facet_it->color = CGAL::Color(0, 255, 0);

	  }
	  else if (/*segment_property_map1[facet_it]*/faceSegmentArray[countOfFaces] == 2){
		  facet_it->color = CGAL::Color(0, 0, 255);

	  }
	  else if (/*segment_property_map1[facet_it]*/faceSegmentArray[countOfFaces] == 3){
		  facet_it->color = CGAL::Color(255, 255, 0);

	  }
	  else if (/*segment_property_map1[facet_it]*/faceSegmentArray[countOfFaces] == 4){
		  facet_it->color = CGAL::Color(255, 0, 255);

	  }
	  else if (/*segment_property_map1[facet_it]*/faceSegmentArray[countOfFaces] == 5){
		  facet_it->color = CGAL::Color(0, 255, 255);

	  }
	  else if (/*segment_property_map1[facet_it]*/faceSegmentArray[countOfFaces] == 6){
		  facet_it->color = CGAL::Color(255, 255, 255);

	  }
	  else if (/*segment_property_map1[facet_it]*/faceSegmentArray[countOfFaces] == 7){
		  facet_it->color = CGAL::Color(0, 0, 0);

	  }
	  else if (/*segment_property_map1[facet_it]*/faceSegmentArray[countOfFaces] == 8){
		  facet_it->color = CGAL::Color(255, 125, 125);

	  }
	  else if (/*segment_property_map1[facet_it]*/faceSegmentArray[countOfFaces] == 6){
		  facet_it->color = CGAL::Color(125, 255, 125);
	  }
	  else if (/*segment_property_map1[facet_it]*/faceSegmentArray[countOfFaces] == 7){
		  facet_it->color = CGAL::Color(125, 125, 255);
	  }
	  else if (/*segment_property_map1[facet_it]*/faceSegmentArray[countOfFaces] == 8){
		  facet_it->color = CGAL::Color(0, 125, 125);
	  }
	  else if (/*segment_property_map1[facet_it]*/faceSegmentArray[countOfFaces] == 9){
		  facet_it->color = CGAL::Color(125, 125, 125);
	  }
	  else if (/*segment_property_map1[facet_it]*/faceSegmentArray[countOfFaces] == 10){
		  facet_it->color = CGAL::Color(50, 0, 255);
	  }
	  else if (/*segment_property_map1[facet_it]*/faceSegmentArray[countOfFaces] == 11){
		  facet_it->color = CGAL::Color(0, 50, 255);
	  }
	  else if (/*segment_property_map1[facet_it]*/faceSegmentArray[countOfFaces] == 12){
		  facet_it->color = CGAL::Color(0, 255, 50);
	  }
	  else if (/*segment_property_map1[facet_it]*/faceSegmentArray[countOfFaces] == 13){
		  facet_it->color = CGAL::Color(50, 255, 0);
	  }
	  else if (/*segment_property_map1[facet_it]*/faceSegmentArray[countOfFaces] == 14){
		  facet_it->color = CGAL::Color(255, 0, 50);
	  }
	  else if (/*segment_property_map1[facet_it]*/faceSegmentArray[countOfFaces] == 15){
		  facet_it->color = CGAL::Color(50, 0, 50);
	  }
	  else if (/*segment_property_map1[facet_it]*/faceSegmentArray[countOfFaces] == 16){
		  facet_it->color = CGAL::Color(0, 50, 50);
	  }
	  else if (/*segment_property_map1[facet_it]*/faceSegmentArray[countOfFaces] == 17){
		  facet_it->color = CGAL::Color(50, 50, 0);
	  }
	  else if (/*segment_property_map1[facet_it]*/faceSegmentArray[countOfFaces] == 18){
		  facet_it->color = CGAL::Color(100, 150, 250);
	  }
	  else if (/*segment_property_map1[facet_it]*/faceSegmentArray[countOfFaces] == 19){
		  facet_it->color = CGAL::Color(250, 150, 100);
	  }
	  else if (/*segment_property_map1[facet_it]*/faceSegmentArray[countOfFaces] == 20){
		  facet_it->color = CGAL::Color(250, 150, 100);
	  }
	  else if (/*segment_property_map1[facet_it]*/faceSegmentArray[countOfFaces] == 21){
		  facet_it->color = CGAL::Color(250, 150, 100);
	  }
	  else if (/*segment_property_map1[facet_it]*/faceSegmentArray[countOfFaces] == 22){
		  facet_it->color = CGAL::Color(250, 150, 100);
	  }
	  ++countOfFaces;
	 
  }

 
  int vertexFaceCountIndex = 0;
  std::string line;
  std::ifstream input2("C:\\Users\\PC1\\AppData\\Roaming\\Skype\\My Skype Received Files\\3dscansAndo\\3dscansAndo\\IndraneelData\\female5.off");
  //input.open("C:\\Users\\PC1\\AppData\\Roaming\\Skype\\My Skype Received Files\\3dscansAndo\\3dscansAndo\\ColoredAndo.off");
  if (input2.is_open())
  {
	  while (std::getline(input2, line))
	  {
		  //  std::cout << "read file  LAst" << std::endl;
		  ++vertexFaceCountIndex;

		 // std::cout << "read file size Info Artash  " << std::endl;
		 
		  if (vertexFaceCountIndex == 2){
			  out << line << '\n';
			  break;
		  }
	  }
	  input2.close();
  }

  //out << countOfVertexes << " " << countOfFaces << " " << 0 << "\n";
  int countOfWritedVertexes = 0;
  for (Polyhedron1::Vertex_iterator vert_it = P.vertices_begin(); vert_it != P.vertices_end(); ++vert_it) {
	  //vert_it->halfedge()->facet()->
	  double vx = vert_it->point().x();
	  double vy = vert_it->point().y();
	  double vz = vert_it->point().z();
	
	  //std::cout << "Current vertex " << vx << "   " << vy << "   " << vz << std::endl;
	  if (countOfWritedVertexes <= countOfVertexes) {
		  out << vx << " " << vy << " " << vz << " " << (int)vert_it->halfedge()->facet()->color.r() << " " << (int)vert_it->halfedge()->facet()->color.g() << " " << (int)vert_it->halfedge()->facet()->color.b() << " " << 255 << "\n";
		 //std::cout << "First face color " << (int)vert_it->halfedge()->facet()->color.r() << " " << (int)vert_it->halfedge()->facet()->color.g() << " " << (int)vert_it->halfedge()->facet()->color.b() << std::endl;
	  }
	  ++countOfWritedVertexes;
	  //break;
  }
  int lineIndex = 0;

  input2.open("C:\\Users\\PC1\\AppData\\Roaming\\Skype\\My Skype Received Files\\3dscansAndo\\3dscansAndo\\IndraneelData\\female5.off");
  //input.open("C:\\Users\\PC1\\AppData\\Roaming\\Skype\\My Skype Received Files\\3dscansAndo\\3dscansAndo\\ColoredAndo.off");
  if (input2.is_open())
  {
	  while (std::getline(input2, line))
	  {
		//  std::cout << "read file  LAst" << std::endl;
		  ++lineIndex;
		 
		  if (lineIndex >= countOfWritedVertexes + 3){
			   out << line << '\n';
		  }
	  }
	  input2.close();
  }
  /*Artash changed end*/
}

