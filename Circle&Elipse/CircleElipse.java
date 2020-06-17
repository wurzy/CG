import java.util.*;
public class CircleElipse {
    static void elipse(){
        float a = 70;
        float b = 25;
        float h = 36;
        float t = 0;
        double x, z;
        double step = Math.PI/6;
        int steps = 12;
        double ang = 0;
        System.out.println("***********************");
        for(int i = 0; i < steps; i++){
            x = h + a * Math.cos(ang);
            z = t + b * Math.sin(ang);
            ang+=step;
            System.out.println("      <point " + "x=\"" + x + "\" " + "y=\"" + 0 + "\" " + "z=\"" + z + "\"" + "/>");
        }
        System.out.println("***********************");
    }

    static void circ(){
        double step = Math.PI/6;
        double ang = 0;
        int steps = 12;
        float rad = 7;
        double x, z;
        System.out.println("***********************");
        for(int i = 0; i < steps; i++){
            x = rad * Math.cos(ang);
            z = rad * Math.sin(ang);
            ang+=step;
            System.out.println("      <point " + "x=\"" + x + "\" " + "y=\"" + 0 + "\" " + "z=\"" + z + "\"" + "/>");
        }
        System.out.println("***********************");
    }

    public static void main(String... args){
        circ();
        //elipse();
        meio();
    }
}
