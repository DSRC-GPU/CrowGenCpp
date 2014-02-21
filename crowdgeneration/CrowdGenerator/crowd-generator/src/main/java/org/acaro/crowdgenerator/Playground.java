package org.acaro.crowdgenerator;

import java.util.Random;

import org.apache.commons.math3.geometry.euclidean.oned.Interval;

public class Playground {
	
	private static float movingP(float lambda) {
		Random rn = new Random();
		// random number with exponential distirbution
		return (float) Math.log(1 - rn.nextFloat()) / (- lambda);
	}
	public static void main(String[] args) {
		Interval i = new Interval(10, 1000);
		System.out.println("Interval: " +
						   " lower=" + i.getLower() +
						   " midpoint=" + i.getMidPoint() +
						   " upper=" + i.getUpper() +
						   " length=" + i.getLength());
	}
}
