Class {
	#name : #AGPUTextureViewDescription,
	#pools : [
		'AGPUConstants'
	],
	#superclass : #ExternalStructure,
	#category : 'AbstractGPU-GeneratedSqueak'
}

{ #category : #'definition' }
AGPUTextureViewDescription class >> fields [
	"
	self defineFields
	"
    ^ #(
		(type 'long')
		(texture 'void*')
		(format 'long')
		(sample_count 'ulong')
		(components 'AGPUComponentsSwizzle')
		(subresource_range 'AGPUSubresourceRange')
	)
]

