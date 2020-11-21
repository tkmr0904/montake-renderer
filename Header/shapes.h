#ifndef SHAPES_H
#define SHAPES_H

#include "hit.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void checkPattern(int const& shapenum, int const& patternnum);//適切なPatternかどうか調べる

class Shapes
{
    public:

    virtual void intersect0(const Ray& ray, Hit& hit0) const = 0;
    virtual void intersect(const Ray& ray, Hit& hit0) const = 0;
    virtual void intersect0_nee(const Ray& ray, Hit& hit0, double& pdfsum_nee_1_obj) const = 0;
    virtual void intersect_nee(const Ray& ray, Hit& hit0, double& pdfsum_nee_1_obj) const = 0;
    virtual double getpdf_nee_1_obj() const = 0;
    virtual Vec3 changeposition(Vec3 p) const = 0;
    virtual Vec3 rechangeposition(Vec3 p) const = 0;
    virtual Vec3 changedirection(Vec3 p) const = 0;
    virtual Vec3 rechangedirection(Vec3 p) const = 0;
    virtual Ray changeray (Ray ray)const = 0;
    virtual Ray rechangeray (Ray ray)const = 0;
    virtual void getposition(Hit& hit) const = 0;
    virtual Vec3 pos() const = 0;
    virtual void moveShape(const Matrix3& rotatemat, const Matrix3& rerotatemat, const Vec3& movedirection) = 0;
    virtual int shapenum()const = 0;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Sphere : public Shapes
{
    public:
    const double R;
    Vec3 Center;
    std::shared_ptr<L::Pattern> ppattern_l;
    std::shared_ptr<M::Pattern> ppattern_m;

    Matrix3 changemat;
    Matrix3 rechangemat;
    
    Sphere (double const& R, double const& theta, double const& phi, double const& alpha, Vec3 const& Center, std::shared_ptr<L::Pattern> const& ppattern_l, std::shared_ptr<M::Pattern> const& ppattern_m);

    void intersect0(const Ray& ray, Hit& hit0) const;
    void intersect(const Ray& ray, Hit& hit0) const;
    void intersect0_nee(const Ray& ray, Hit& hit0, double& pdfsum_nee_1_obj) const;
    void intersect_nee(const Ray& ray, Hit& hit0, double& pdfsum_nee_1_obj) const;
    double getpdf_nee_1_obj() const;
    Vec3 changeposition(Vec3 p) const;
    Vec3 rechangeposition(Vec3 p) const;
    Vec3 changedirection(Vec3 p) const;
    Vec3 rechangedirection(Vec3 p) const;
    Ray changeray (Ray ray)const;
    Ray rechangeray (Ray ray)const;
    Vec3 getnormal0(Vec3 position0) const;
    Vec3 getnormal(Vec3 position) const;
    void getposition(Hit& hit) const;
    Vec3 pos() const;
    void moveShape(const Matrix3& rotatemat, const Matrix3& rerotatemat, const Vec3& movedirection);
    int shapenum()const;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Pipe : public Shapes
{
    public:
    const double radius;
    Vec3 position;
    std::shared_ptr<L::Pattern>ppattern_l;
    std::shared_ptr<M::Pattern> ppattern_m;
    double length;
    Matrix3 changemat;
    Matrix3 rechangemat;
    

    Pipe(const double radius, const Vec3 position, const Vec3 direction, const double alpha, const std::shared_ptr<L::Pattern> ppattern_l, std::shared_ptr<M::Pattern> ppattern_m);

    void intersect0(const Ray& ray, Hit& hit0) const;
    void intersect(const Ray& ray, Hit& hit0) const;
    void intersect0_nee(const Ray& ray, Hit& hit0, double& pdfsum_nee_1_obj) const;
    void intersect_nee(const Ray& ray, Hit& hit0, double& pdfsum_nee_1_obj) const;
    double getpdf_nee_1_obj() const;
    bool ifinside0(Vec3 p);
    bool ifinside(Vec3 p);
    Vec3 changeposition(Vec3 p) const;
    Vec3 rechangeposition(Vec3 p) const;
    Vec3 changedirection(Vec3 p) const;
    Vec3 rechangedirection(Vec3 p) const;
    Ray changeray (Ray ray)const;
    Ray rechangeray (Ray ray)const;
    Vec3 getnormal0(Vec3 position0) const;
    Vec3 getnormal(Vec3 position) const;
    void getposition(Hit& hit) const;
    Vec3 pos() const;
    void moveShape(const Matrix3& rotatemat, const Matrix3& rerotatemat, const Vec3& movedirection);
    int shapenum()const;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Triangle : public Shapes
{
    public:
    Vec3 point0;
    Vec2 p1;
    Vec2 p2;
    Matrix2 M;//逆行列
    std::shared_ptr<L::Pattern>ppattern_l;
    std::shared_ptr<M::Pattern> ppattern_m;
    Vec3 normal;
    double sin_p1_p2;

    Matrix3 changemat;
    Matrix3 rechangemat;


    Triangle(Vec3 point0, Vec3 point1, Vec3 point2, double alpha, std::shared_ptr<L::Pattern> ppattern_l, std::shared_ptr<M::Pattern> ppattern_m);

    void intersect0(const Ray& ray, Hit& hit0) const;
    void intersect(const Ray& ray, Hit& hit0) const;
    void intersect0_nee(const Ray& ray, Hit& hit0, double& pdfsum_nee_1_obj) const;
    void intersect_nee(const Ray& ray, Hit& hit0, double& pdfsum_nee_1_obj) const;
    double getpdf_nee_1_obj() const;
    Vec3 changeposition(Vec3 p) const;
    Vec3 rechangeposition(Vec3 p) const;
    Vec3 changedirection(Vec3 p) const;
    Vec3 rechangedirection(Vec3 p) const;
    Ray changeray (Ray ray)const;
    Ray rechangeray (Ray ray)const;
    Vec3 getnormal0(Vec3 position0) const;
    Vec3 getnormal(Vec3 position) const;
    void getposition(Hit& hit) const;
    Vec3 pos() const;
    void moveShape(const Matrix3& rotatemat, const Matrix3& rerotatemat, const Vec3& movedirection);
    int shapenum()const;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Disk :public Shapes
{
    public:
    const double R;
    Vec3 Center;

    std::shared_ptr<L::Pattern>ppattern_l;
    std::shared_ptr<M::Pattern> ppattern_m;

    Matrix3 changemat;
    Matrix3 rechangemat;

    Disk(double R, Vec3 Center, double phi, double theta, double alpha, std::shared_ptr<L::Pattern> ppattern_l, std::shared_ptr<M::Pattern> ppattern_m);
    void intersect0(const Ray& ray, Hit& hit0) const;
    void intersect(const Ray& ray, Hit& hit0) const;
    void intersect0_nee(const Ray& ray, Hit& hit0, double& pdfsum_nee_1_obj) const;
    void intersect_nee(const Ray& ray, Hit& hit0, double& pdfsum_nee_1_obj) const;
    double getpdf_nee_1_obj() const;
    Vec3 changeposition(Vec3 p) const;
    Vec3 rechangeposition(Vec3 p) const;
    Vec3 changedirection(Vec3 p) const;
    Vec3 rechangedirection(Vec3 p) const;
    Ray changeray (Ray ray)const;
    Ray rechangeray (Ray ray)const;
    Vec3 getnormal0(Vec3 position0) const;
    Vec3 getnormal(Vec3 position) const;
    void getposition(Hit& hit) const;
    Vec3 pos() const;
    void moveShape(const Matrix3& rotatemat, const Matrix3& rerotatemat, const Vec3& movedirection);
    int shapenum()const;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Cuboid :public Shapes
{
    public:

    /************************辺の長さに関する***********************************************************************************/
    const double x_length;
    const double y_length;
    const double z_length;

    double const length[3][2] = {{x_length/2, -x_length/2}, {y_length/2, -y_length/2}, {z_length/2, -z_length/2}};
    /***************************************************************************************************************/

    

    /**************************放射される光に関する********************************************************************************/
    std::shared_ptr<L::Pattern> ppattern_l[3][2];
    /***************************************************************************************************************/


    /*****************************Patternに関する*************************************************************************/
    std::shared_ptr<M::Pattern> ppattern_m[3][2];
    /***************************************************************************************************************/

    Matrix3 changemat;
    Matrix3 rechangemat;

    Vec3 Center;
    Vec3 normal_[3][2] = {{Vec3(1,0,0), Vec3(-1,0,0)}, {Vec3(0,1,0), Vec3(0,-1,0)}, {Vec3(0,0,1), Vec3(0,0,-1)}};



    Cuboid(double x_length, double y_length, double z_length, Vec3 Center, double theta, double phi, double alpha, std::shared_ptr<L::Pattern> ppattern_l_x_u, std::shared_ptr<L::Pattern> ppattern_l_x_d, std::shared_ptr<L::Pattern> ppattern_l_y_u, std::shared_ptr<L::Pattern> ppattern_l_y_d, std::shared_ptr<L::Pattern> ppattern_l_z_u, std::shared_ptr<L::Pattern> ppattern_l_z_d, std::shared_ptr<M::Pattern> ppattern_x_u, std::shared_ptr<M::Pattern> ppattern_x_d, std::shared_ptr<M::Pattern> ppattern_y_u, std::shared_ptr<M::Pattern> ppattern_y_d, std::shared_ptr<M::Pattern> ppattern_z_u, std::shared_ptr<M::Pattern> ppattern_z_d);
    
    void intersect0(const Ray& ray, Hit& hit0) const;
    void intersect(const Ray& ray, Hit& hit0) const;
    void intersect0_nee(const Ray& ray, Hit& hit0, double& pdfsum_nee_1_obj) const;
    void intersect_nee(const Ray& ray, Hit& hit0, double& pdfsum_nee_1_obj) const;
    double getpdf_nee_1_obj() const;
    bool ifinside0(Vec3 p);
    bool ifinside(Vec3 p);
    Vec3 changeposition(Vec3 p) const;
    Vec3 rechangeposition(Vec3 p) const;
    Vec3 changedirection(Vec3 p) const;
    Vec3 rechangedirection(Vec3 p) const;
    Ray changeray (Ray ray)const;
    Ray rechangeray (Ray ray)const;
    void getposition(Hit& hit) const;
    Vec3 pos() const;
    void moveShape(const Matrix3& rotatemat, const Matrix3& rerotatemat, const Vec3& movedirection);
    int shapenum()const;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Objects
{
    public:

    std::vector<std::shared_ptr<Shapes>> shapes[(int)SHAPES::NUM_OF_SHAPES];
    std::vector<std::shared_ptr<Shapes>> shapes_nee[(int)SHAPES::NUM_OF_SHAPES];


    int objectnumber[(int)SHAPES::NUM_OF_SHAPES];     //物体の個数(種類別)
    int objectnumber_nee[(int)SHAPES::NUM_OF_SHAPES]; //neeの物体の個数(種類別)
    

    int objectnumbersum;//nee以外の物体の個数
    int objectnumbersum_nee;//neeの物体の個数

    Objects();

    Objects(const Objects&) = delete;//コピーの禁止

    void addshape(std::shared_ptr<Shapes> shape1);

    void addshape_nee(std::shared_ptr<Shapes> shape1);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void moveStructure(Objects& obj0, Objects& obj, Matrix3 const& rotatemat, Matrix3 const& rerotatemat, Vec3 const& movedirection);

class Structures
{
    public:
    virtual void putStructure(Objects& objects)const = 0;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Cylinder : public Structures
{
    public:
    M::Colorful_pipe p1 = M::Colorful_pipe();
    M::Colorful_disk p2 = M::Colorful_disk();

    L::sharePattern q1 = L::pPlain(L::Uniform(Vec3(0,0,0)));


    double radius;
    double length;

    Matrix3 rotatemat;
    Matrix3 rerotatemat;
    Vec3 movedirection;


    Cylinder(const double radius, const Vec3 position, const Vec3 direction, const double alpha);

    Cylinder(const double radius, const double length, const Vec3 position, const double theta, const double phi, const double alpha);

    Cylinder();

    void getstructure0(Objects& obj0) const;
    void putStructure(Objects& objects)const;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Cylinder_center : public Structures
{
    public:
    M::Colorful_pipe p1 = M::Colorful_pipe();
    M::Colorful_disk p2 = M::Colorful_disk();

    L::sharePattern q1 = L::pPlain(L::Uniform(Vec3(0,0,0)));

    double radius;
    double length;

    Matrix3 rotatemat;
    Matrix3 rerotatemat;
    Vec3 movedirection;



    Cylinder_center(const double radius, const double length, const Vec3 position, const double theta, const double phi, const double alpha);

    Cylinder_center();
    void getstructure0(Objects& obj0) const;
    void putStructure(Objects& objects)const;
};

void moveStructure(Objects& obj0, Objects& obj, Matrix3 const& rotatemat, Matrix3 const& rerotatemat, Vec3 const& movedirection);

////////////////////////////////////////////////////////////////////////////////////////////////

Vec3 AtMost255(Vec3 v);



class Accel
{
    public:

    Accel();


    void intersects(const Ray& ray, const Objects& objects, Hit& hit);


    void intersects_nee1(const Ray& ray, const Objects& objects, Hit& hit);


    void intersects_nee2(const Ray& ray, const Objects& objects, Hit& hit, double& pdfsum_nee);
};



#endif